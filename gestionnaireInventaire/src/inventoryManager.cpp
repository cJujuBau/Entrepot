#include <iostream>
#include <fstream>
#include <vector>
#include <ncurses.h>
#include <nlohmann/json.hpp>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

#include <utils.h>
#include <path.h>

using json = nlohmann::json;

#define WIDTH 60
#define NB_MORE_ROW 4

#define WIDTH_SUB_WINDOW 40
#define HEIGHT_SUB_WINDOW 5
#define START_WIDTH_SUB_WINDOW 5
#define START_HEIGHT_SUB_WINDOW 10

#define ENTER_KEY 10

struct Item {
    int id;
    std::string name;
    int quantity;
    int aisle; // Nul d'avoir ca dans cette structure 
    int row;
};

class InventoryManager {
private:
    std::vector<Item> items;
    json inventoryJson;
    std::string filePath;
    std::vector<std::pair<int, int>> selectedItems; // (index, selected quantity)

    // Pointer for shared memory
    void* shm_ptr;
    int pidMain = 999999;

public:
    InventoryManager(const std::string& file) : filePath(file), shm_ptr(nullptr) {
        loadInventory();
        openSharedMemory();
        getMainPID();
    }

    ~InventoryManager() {
        // Close and destroy shared memory at the end
        if (shm_ptr) {
            CHECK(munmap(shm_ptr, SHM_SIZE), "Failed to unmap shared memory");
        }
        CHECK(shm_unlink(SHM_ORDER), "Failed to unlink shared memory");
    }

    void openSharedMemory() {
        // Open or create the shared memory segment
        int shm_fd = shm_open(SHM_ORDER, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        CHECK(shm_fd, "Failed to open shared memory");

        // Associate the memory segment with the process's address space
        CHECK(ftruncate(shm_fd, SHM_SIZE), "Failed to set the size of shared memory");

        shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        CHECK_NULL(shm_ptr, "Failed to map shared memory");

        // Close the file descriptor after mapping
        CHECK(close(shm_fd), "Failed to close shared memory file descriptor");
    }

    // Get PID of the server to kill him after sending the order
    void getMainPID() {
        std::ifstream file(PATH_FPID_MAIN);
        if (!file) {
            std::cerr << "Failed to open main PID file!" << std::endl;
            exit(EXIT_FAILURE);
        }
        file >> pidMain;
        CHECK(pidMain, "Failed to read main PID");
    }

    void sendOrder() {
        // Format the order as a string
        std::string orderStr = "{";
        
        for (const auto& [index, qty] : selectedItems) {
            orderStr += "{" + std::to_string(items[index].id) + ":" + std::to_string(qty) + "}, ";
        }
        orderStr.pop_back();
        orderStr.pop_back();
        orderStr += "}";


        if (shm_ptr) {
            // Copy the order to shared memory
            memcpy(shm_ptr, orderStr.c_str(), orderStr.size() + 1);

            // Wake up the server
            kill(pidMain, SIGUSR1);
        } else {
            std::cerr << "Shared memory is not open!" << std::endl;
        }
    }

    // Get items from the JSON file
    void loadInventory() {
        std::ifstream file(filePath);
        if (!file) {
            std::cerr << "Failed to open JSON file!" << std::endl;
            return;
        }
        file >> inventoryJson;

        
        items.clear();

        for (const auto& obj : inventoryJson) {
            items.push_back({obj["id"], obj["name"], obj["quantity"], obj["row"], obj["aisle"] });
        }
    }

    // Save the updated inventory to the JSON file after each order
    void saveInventory() {
        for (const auto& [index, qty] : selectedItems) {
            inventoryJson[index]["quantity"] = inventoryJson[index]["quantity"].get<int>() - qty;
        }
        std::ofstream file(filePath);
        file << inventoryJson.dump(4);
    }

    // Display the inventory in a window
    void displayInventory(WINDOW* win, int highlight) {
        werase(win);
        box(win, 0, 0);
        for (int i = 0; i < static_cast<int>(items.size()); ++i) {
            if (i == highlight) wattron(win, A_REVERSE);
            mvwprintw(win, i + 1, 1, "%d - %s (Qty: %d)", items[i].id, items[i].name.c_str(), items[i].quantity);
            if (i == highlight) wattroff(win, A_REVERSE);
        }
        // Add instructions at the bottom
        mvwprintw(win, items.size() + 2, 1, "[Enter] Choose  [v] Validate  [q] Quit");
        
        wrefresh(win);
    }

    // Display a window to confirm or cancel the order
    void showValidationWindow() {
        WINDOW* valWin = newwin(HEIGHT_SUB_WINDOW, WIDTH_SUB_WINDOW, START_WIDTH_SUB_WINDOW, START_HEIGHT_SUB_WINDOW);
        box(valWin, 0, 0);
        mvwprintw(valWin, 1, 1, "Confirm selection?");
        mvwprintw(valWin, 2, 1, "[Y]es  [N]o");
        wrefresh(valWin);
        
        int choice;
        while ((choice = wgetch(valWin))) {
            if (choice == 'y' || choice == 'Y') {
                saveInventory();
                sendOrder();

                // Reload the inventory after sending the order to update the quantities
                loadInventory();
                selectedItems.clear(); // a voir
                break;
            }
            if (choice == 'n' || choice == 'N') {
                break;
            }
        }
        delwin(valWin);
    }
    
    // Select items from the inventory
    void selectItems() {
        initscr();
        noecho();
        curs_set(0);
        WINDOW* win = newwin(items.size() + NB_MORE_ROW, WIDTH, 1, 1); // WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);

        keypad(win, TRUE);
        int choice = 0;
        int highlight = 0;
    
        while (true) {
            displayInventory(win, highlight);
            choice = wgetch(win);
            if (choice == KEY_UP && highlight > 0) --highlight;
            if (choice == KEY_DOWN && highlight < (int)items.size() - 1) ++highlight;
            if (choice == ENTER_KEY) {  // Enter key
                int qty = getQuantity(win, items[highlight].name, items[highlight].quantity);
                if (qty > 0 && qty <= items[highlight].quantity) {
                    selectedItems.push_back({highlight, qty});
                }
            }
            if (choice == 'q') break;
            if (choice == 'v') {
                showValidationWindow();
                
            }
        }
        endwin();
    }

    // Get the quantity of the selected item in a new window
    int getQuantity(WINDOW* win, const std::string& itemName, int availableQty) {
        int qty = 0;
        while (true) {
            werase(win);
            box(win, 0, 0); // box(WINDOW *win, chtype verch, chtype horch);
            mvwprintw(win, 1, 1, "Enter quantity for %s: ", itemName.c_str());
            wrefresh(win);
            echo();
            curs_set(1);
            wscanw(win, "%d", &qty);
            noecho();
            curs_set(0);
            
            // Check if the quantity is valid
            if (qty > availableQty) showErrorWindow("Quantity exceeds available stock!");
            else if (qty <= 0) {
                showErrorWindow("Invalid quantity!");
                return -1;
            }
            else break;
            
        }
        return qty;
    }

    // Error window to display messages and wait for user input
    void showErrorWindow(const std::string& message) {
        WINDOW* errWin = newwin(HEIGHT_SUB_WINDOW, WIDTH_SUB_WINDOW, START_WIDTH_SUB_WINDOW, START_HEIGHT_SUB_WINDOW);
        box(errWin, 0, 0); // box(WINDOW *win, chtype verch, chtype horch);

        mvwprintw(errWin, 1, 1, "%s", message.c_str()); // mvwprintw(WINDOW *win, int y, int x, const char *fmt, ...);
        mvwprintw(errWin, 3, 1, "Press [Enter] to continue");
        wrefresh(errWin);
    
        while (true) {
            int ch = wgetch(errWin);
            if (ch == ENTER_KEY) {
                break;
            }
        }
    
        delwin(errWin);
    }
};

int main() {
    InventoryManager manager(PATH_INVENTORY);
    manager.selectItems();
    return 0;
}
