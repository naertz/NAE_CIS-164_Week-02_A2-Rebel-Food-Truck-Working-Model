//================================================================================
// Name        : rebel_food_truck_inventory_sales.cpp
// Author      : Noah Allan Ertz (NAE)
// Email       : naertz@dmacc.edu
// Date        : 2021-09-12
// Description : Working Model of the Rebel Food Truck Inventory and Sales Program
//================================================================================

#include <climits>
#include <cmath>
#include <cstdlib>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// Global Variable(s) ========================================================================================
enum stringToIntegerError { STRTOINT_SUCCESS, STRTOINT_OVERFLOW, STRTOINT_UNDERFLOW, STRTOINT_INCONVERTIBLE };
// ===========================================================================================================

int getLongestStringLength(const std::vector<std::string>& tableStrings);
stringToIntegerError stringToIntegerValidation (int &parsedInteger, const char *stringInputPointer, int base = 0);
int getValidInteger(std::string stringInput, int minValue, int maxValue, int messageType = 0);

// Current model only displays hamburgers, hotdogs, their chili versions, and chili.
int main() {
    // Food truck max capacities for each ingredient
    const int HAMBURGER_PATTY_CAPACITY = 200;
    const int HAMBURGER_BUN_CAPACITY   =  75;
    const int HOTDOG_CAPACITY          = 200;
    const int HOTDOG_BUN_CAPACITY      =  75;
    const int CHILI_CAPACITY           = 500;

    // Serving amount for each chili type
    const int CHILI_SELF_SERVING  = 12;
    const int CHILI_ADDON_SERVING =  4;

    // Low inventory thresholds
    const double LOW_INVENTORY_THRESHOLD       = 0.2;
    const int HAMBURGER_PATTY_LOW_INVENTORY    = static_cast<int>(std::floor(static_cast<double>(HAMBURGER_PATTY_CAPACITY) * LOW_INVENTORY_THRESHOLD));
    const int HAMBURGER_BUN_LOW_INVENTORY      = static_cast<int>(std::floor(static_cast<double>(HAMBURGER_BUN_CAPACITY)   * LOW_INVENTORY_THRESHOLD));
    const int HOTDOG_LOW_INVENTORY             = static_cast<int>(std::floor(static_cast<double>(HOTDOG_CAPACITY)          * LOW_INVENTORY_THRESHOLD));
    const int HOTDOG_BUN_LOW_INVENTORY         = static_cast<int>(std::floor(static_cast<double>(HOTDOG_BUN_CAPACITY)      * LOW_INVENTORY_THRESHOLD));
    const int CHILI_LOW_INVENTORY              = static_cast<int>(std::floor(static_cast<double>(CHILI_CAPACITY)           * LOW_INVENTORY_THRESHOLD));

    // Empty inventory
    const int EMPTY_INVENTORY = 0;

    // Item prices
    const double HAMBURGER_PRICE  = 5.00;
    const double HOTDOG_PRICE     = 5.00;
    const double CHILI_SELF_PRICE = 4.00;

    // Chili addon prices
    const double CHILI_ADDON_PRICE = 2.00;
    const double CHILIBURGER_PRICE = HAMBURGER_PRICE + CHILI_ADDON_PRICE;
    const double CHILIDOG_PRICE    = HOTDOG_PRICE    + CHILI_ADDON_PRICE;

    // Sales tax
    const double SALES_TAX = 0.05;

    // Option selections initialized for while loops
    int mainOptionSelection      = -1;
    int inventoryOptionSelection = -1;
    int sellOptionSelection      = -1;

    // Current ingredient inventories
    int currentHamburgerPattyInventory = HAMBURGER_PATTY_CAPACITY;
    int currentHamburgerBunInventory   = HAMBURGER_BUN_CAPACITY;
    int currentHotdogInventory         = HOTDOG_CAPACITY;
    int currentHotdogBunInventory      = HOTDOG_BUN_CAPACITY;
    int currentChiliInventory          = CHILI_CAPACITY;

    // Chili servings available
    int currentChiliSelfServingsAvailable  = currentChiliInventory / CHILI_SELF_SERVING;
    int currentChiliAddonServingsAvailable = currentChiliInventory / CHILI_ADDON_SERVING;

    // Potentially new ingredient inventories to update current ingredient inventories
    int newHamburgerPattyInventory;
    int newHamburgerBunInventory;
    int newHotdogInventory;
    int newHotdogBunInventory;
    int newChiliInventory;

    // Max quantity of each item available to sell
    int maxQuantityOfHamburgersToSell;
    int maxQuantityOfHotdogsToSell;
    int maxQuantityOfChiliSelfToSell;

    // Max quantity of each item with chili addon available to sell
    int maxQuantityOfChiliburgersToSell;
    int maxQuantityOfChilidogsToSell;

    // Existence of items initialized for if statements
    bool isThereHamburger   = true;
    bool isThereChiliburger = true;
    bool isThereHotdog      = true;
    bool isThereChilidog    = true;
    bool isThereChiliSelf   = true;

    // Quantities of each item that are currently being ordered
    int quantityOfHamburgersToSell;
    int quantityOfHotdogsToSell;
    int quantityOfChiliSelfToSell;

    // Quantities of each item with chili addon that are currently being ordered
    int quantityOfChiliburgersToSell;
    int quantityOfChilidogsToSell;

    // Cost of current item(s) sold
    double costOfItemsSold;

    // Cost totals
    double orderSubtotal;
    double orderTax;
    double orderTotal;

    // Print title of the program.
    std::cout << "Rebel Food Truck Inventory Sales Program" << std::endl;

    // Vector of strings for main options to use to determine dynamic padding
    std::vector<std::string> mainNumberColumn = { "#", "0", "1", "2" };
    std::vector<std::string> mainOptionColumn = { "Option", "Inventory", "Sell", "Quit" };

    // Length of longest string of each main option column
    const int LONGEST_MAIN_NUMBER_LENGTH = getLongestStringLength(mainNumberColumn);
    const int LONGEST_MAIN_OPTION_LENGTH = getLongestStringLength(mainOptionColumn);

    // Lengths of each width for padding
    const int MAIN_NUMBER_WIDTH = LONGEST_MAIN_NUMBER_LENGTH;
    const int MAIN_OPTION_WIDTH = LONGEST_MAIN_OPTION_LENGTH + 4;

    // String input
    std::string stringInput = "";

    // Execute while main option to quit is not selected.
    while (mainOptionSelection != 2) {
        // Print formatted table.
        std::stringstream mainOptionOSS;
        mainOptionOSS << std::endl;
        for (unsigned int i = 0; i < mainNumberColumn.size(); ++i) {
            mainOptionOSS << std::left << std::setw(MAIN_NUMBER_WIDTH) << mainNumberColumn.at(i) << std::right << std::setw(MAIN_OPTION_WIDTH) << mainOptionColumn.at(i) << std::endl;
        }
        mainOptionOSS << std::endl;
        std::cout << mainOptionOSS.str();

        // Execute until valid integer is parsed.
        do {
            // Prompt user for main option selection.
            std::cout << "Enter option: " << std::flush;

            // Get string input.
            std::getline(std::cin, stringInput);

            // Validate input.
            mainOptionSelection = getValidInteger(stringInput, std::stoi(mainNumberColumn.at(1)), std::stoi(mainNumberColumn.back()));
        } while (mainOptionSelection == -1);

        // Determine main option selected.
        if (mainOptionSelection == std::stoi(mainNumberColumn.at(1))) { // Inventory menu
            do {
                // String stream to get each string for current inventory column
                std::stringstream inventoryOptionStringsOSS;

                // String for current inventory column heading.
                std::string currentInventoryHeading = "Current Inventory";

                // String for hamburger patty inventory
                inventoryOptionStringsOSS << currentHamburgerPattyInventory;
                std::string currentHamburgerPattyInventoryString = inventoryOptionStringsOSS.str();
                inventoryOptionStringsOSS.str("");
                inventoryOptionStringsOSS.clear();

                // String for hamburger bun inventory
                inventoryOptionStringsOSS << currentHamburgerBunInventory;
                std::string currentHamburgerBunInventoryString = inventoryOptionStringsOSS.str();
                inventoryOptionStringsOSS.str("");
                inventoryOptionStringsOSS.clear();

                // String for hotdog inventory
                inventoryOptionStringsOSS << currentHotdogInventory;
                std::string currentHotdogInventoryString = inventoryOptionStringsOSS.str();
                inventoryOptionStringsOSS.str("");
                inventoryOptionStringsOSS.clear();

                // String for hotdog bun inventory
                inventoryOptionStringsOSS << currentHotdogBunInventory;
                std::string currentHotdogBunInventoryString = inventoryOptionStringsOSS.str();
                inventoryOptionStringsOSS.str("");
                inventoryOptionStringsOSS.clear();

                // String for chili inventory
                inventoryOptionStringsOSS << currentChiliInventory << " oz";
                std::string currentChiliInventoryString = inventoryOptionStringsOSS.str();
                inventoryOptionStringsOSS.str("");
                inventoryOptionStringsOSS.clear();

                // Vector of strings for inventory options to use to determine dynamic padding
                std::vector<std::string> inventoryNumberColumn           = { "#", "0", "1", "2", "3", "4", "5" };
                std::vector<std::string> inventoryItemOptionColumn       = { "Item/Option", "Hamburger Patties", "Hamburger Buns", "Hotdogs", "Hotdog Buns", "Chili", "Return" };
                std::vector<std::string> inventoryCurrentInventoryColumn = { currentInventoryHeading, currentHamburgerPattyInventoryString, currentHamburgerBunInventoryString, currentHotdogInventoryString, currentHotdogBunInventoryString, currentChiliInventoryString };

                // Length of longest string of each inventory option column
                const int LONGEST_INVENTORY_NUMBER_LENGTH            = getLongestStringLength(inventoryNumberColumn);
                const int LONGEST_INVENTORY_ITEM_OPTION_LENGTH       = getLongestStringLength(inventoryItemOptionColumn);
                const int LONGEST_INVENTORY_CURRENT_INVENTORY_LENGTH = getLongestStringLength(inventoryCurrentInventoryColumn);

                // Lengths of each width for padding
                const int INVENTORY_NUMBER_WIDTH            = LONGEST_INVENTORY_NUMBER_LENGTH;
                const int INVENTORY_ITEM_OPTION_WIDTH       = LONGEST_INVENTORY_ITEM_OPTION_LENGTH + 4;
                const int INVENTORY_CURRENT_INVENTORY_WIDTH = LONGEST_INVENTORY_CURRENT_INVENTORY_LENGTH + 4;

                // Print formatted table.
                std::stringstream inventoryOptionOSS;
                inventoryOptionOSS << std::endl;
                for (unsigned int i = 0; i < inventoryNumberColumn.size() - 1; ++i) {
                    inventoryOptionOSS << std::left << std::setw(INVENTORY_NUMBER_WIDTH) << inventoryNumberColumn.at(i) << std::right << std::setw(INVENTORY_ITEM_OPTION_WIDTH) << inventoryItemOptionColumn.at(i) << std::setw(INVENTORY_CURRENT_INVENTORY_WIDTH) << inventoryCurrentInventoryColumn.at(i) << std::endl;
                }
                inventoryOptionOSS << std::left << std::setw(INVENTORY_NUMBER_WIDTH) << inventoryNumberColumn.back() << std::right << std::setw(INVENTORY_ITEM_OPTION_WIDTH) << inventoryItemOptionColumn.back() << std::endl << std::endl;
                std::cout << inventoryOptionOSS.str();

                // Execute until valid integer is parsed.
                do {
                    // Prompt user for inventory option selection.
                    std::cout << "Enter option to update inventory: " << std::flush;

                    // Get string input.
                    std::getline(std::cin, stringInput);

                    // Validate input.
                    inventoryOptionSelection = getValidInteger(stringInput, std::stoi(inventoryNumberColumn.at(1)), std::stoi(inventoryNumberColumn.back()));
                } while (inventoryOptionSelection == -1);

                // Determine ingredient selected.
                if (inventoryOptionSelection == 0) { // Hamburger Patty
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for inventory amount.
                        std::cout << std::endl << "Enter new hamburger patty inventory: " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        newHamburgerPattyInventory = getValidInteger(stringInput, EMPTY_INVENTORY, HAMBURGER_PATTY_CAPACITY, 1);
                    } while (newHamburgerPattyInventory == -1);

                    // Assign new inventory to current inventory.
                    currentHamburgerPattyInventory = newHamburgerPattyInventory;
                } else if (inventoryOptionSelection == 1) { // Hamburger Bun
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for inventory amount.
                        std::cout << std::endl << "Enter new hamburger bun inventory: " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        newHamburgerBunInventory = getValidInteger(stringInput, EMPTY_INVENTORY, HAMBURGER_BUN_CAPACITY, 2);
                    } while (newHamburgerBunInventory == -1);

                    // Assign new inventory to current inventory.
                    currentHamburgerBunInventory = newHamburgerBunInventory;
                } else if (inventoryOptionSelection == 2) { // Hotdog
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for inventory amount.
                        std::cout << std::endl << "Enter new hotdog inventory: " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        newHotdogInventory = getValidInteger(stringInput, EMPTY_INVENTORY, HOTDOG_CAPACITY, 3);
                    } while (newHotdogInventory == -1);

                    // Assign new inventory to current inventory.
                    currentHotdogInventory = newHotdogInventory;
                } else if (inventoryOptionSelection == 3) { // Hotdog Bun
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for inventory amount.
                        std::cout << std::endl << "Enter new hotdog bun inventory: " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        newHotdogBunInventory = getValidInteger(stringInput, EMPTY_INVENTORY, HOTDOG_BUN_CAPACITY, 4);
                    } while (newHotdogBunInventory == -1);

                    // Assign new inventory to current inventory.
                    currentHotdogBunInventory = newHotdogBunInventory;
                } else if (inventoryOptionSelection == 4) { // Chili
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for inventory amount.
                        std::cout << std::endl << "Enter new chili inventory: " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        newChiliInventory = getValidInteger(stringInput, EMPTY_INVENTORY, CHILI_CAPACITY, 5);
                    } while (newChiliInventory == -1);

                    // Assign new inventory to current inventory.
                    currentChiliInventory = newChiliInventory;

                    // Calculate quantity of each chili serving type available.
                    currentChiliSelfServingsAvailable  = currentChiliInventory / CHILI_SELF_SERVING;
                    currentChiliAddonServingsAvailable = currentChiliInventory / CHILI_ADDON_SERVING;
                } else if (inventoryOptionSelection == 5) { // Return
                    // Exit loop.
                    break;
                }
            } while (inventoryOptionSelection != 5);
        } else if (mainOptionSelection == std::stoi(mainNumberColumn.at(2))) { // Sell menu
            // Initialize order subtotal.
            orderSubtotal = 0;

            do {
                // Determine max quantity of hamburgers available to sell based on ingredient with lowest stock.
                if (currentHamburgerPattyInventory > currentHamburgerBunInventory) {
                    maxQuantityOfHamburgersToSell = currentHamburgerBunInventory;
                } else {
                    maxQuantityOfHamburgersToSell = currentHamburgerPattyInventory;
                }

                // Determine max quantity of chiliburgers available to sell based on ingredient with lowest stock.
                if (currentChiliAddonServingsAvailable > maxQuantityOfHamburgersToSell) {
                    maxQuantityOfChiliburgersToSell = maxQuantityOfHamburgersToSell;
                } else {
                    maxQuantityOfChiliburgersToSell = currentChiliAddonServingsAvailable;
                }

                // Determine max quantity of hotdogs available to sell based on ingredient with lowest stock.
                if (currentHotdogInventory > currentHotdogBunInventory) {
                    maxQuantityOfHotdogsToSell = currentHotdogBunInventory;
                } else {
                    maxQuantityOfHotdogsToSell = currentHotdogInventory;
                }

                // Determine max quantity of chilidogs available to sell based on ingredient with lowest stock.
                if (currentChiliAddonServingsAvailable > maxQuantityOfHotdogsToSell) {
                    maxQuantityOfChilidogsToSell = maxQuantityOfHotdogsToSell;
                } else {
                    maxQuantityOfChilidogsToSell = currentChiliAddonServingsAvailable;
                }

                // Determine max quantity of chili available to sell based on servings available.
                maxQuantityOfChiliSelfToSell = currentChiliSelfServingsAvailable;

                // Determine if hamburgers exist.
                if (maxQuantityOfHamburgersToSell > EMPTY_INVENTORY) {
                    isThereHamburger = true;
                } else {
                    isThereHamburger = false;
                }

                // Determine if chiliburgers exist.
                if (maxQuantityOfChiliburgersToSell > EMPTY_INVENTORY) {
                    isThereChiliburger = true;
                } else {
                    isThereChiliburger = false;
                }

                // Determine if hotdogs exist.
                if (maxQuantityOfHotdogsToSell > EMPTY_INVENTORY) {
                    isThereHotdog = true;
                } else {
                    isThereHotdog = false;
                }

                // Determine if chilidogs exist.
                if (maxQuantityOfChilidogsToSell > EMPTY_INVENTORY) {
                    isThereChilidog = true;
                } else {
                    isThereChilidog = false;
                }

                // Determine if chili exists.
                if (maxQuantityOfChiliSelfToSell > EMPTY_INVENTORY) {
                    isThereChiliSelf = true;
                } else {
                    isThereChiliSelf = false;
                }

                // String stream to get each string for item/option column
                std::stringstream sellOptionStringsOSS;

                // String for item/option column heading
                std::string itemOptionHeading = "Item/Option";

                // String for hamburger
                sellOptionStringsOSS << "Hamburger";
                std::string hamburgerString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chiliburger
                sellOptionStringsOSS << "Chiliburger";
                std::string chiliburgerString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for hotdog
                sellOptionStringsOSS << "Hotdog";
                std::string hotdogString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chilidog
                sellOptionStringsOSS << "Chilidog";
                std::string chilidogString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chili
                sellOptionStringsOSS << "Chili (" << CHILI_SELF_SERVING << " oz)";
                std::string chiliSelfString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for return
                std::string returnString = "Return";

                // String for quantity available heading
                std::string quantityAvailableHeading = "Quantity Available";

                // String for max quantity of hamburgers available
                sellOptionStringsOSS << maxQuantityOfHamburgersToSell;
                std::string maxQuantityOfHamburgersToSellString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for max quantity of chiliburgers available
                sellOptionStringsOSS << maxQuantityOfChiliburgersToSell;
                std::string maxQuantityOfChiliburgersToSellString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for max quantity of hotdogs available
                sellOptionStringsOSS << maxQuantityOfHotdogsToSell;
                std::string maxQuantityOfHotdogsToSellString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for max quantity of chilidogs available
                sellOptionStringsOSS << maxQuantityOfChilidogsToSell;
                std::string maxQuantityOfChilidogsToSellString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for max quantity of chili available
                sellOptionStringsOSS << maxQuantityOfChiliSelfToSell;
                std::string maxQuantityOfChiliSelfToSellString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for cost per item heading
                std::string costPerItemHeading = "Cost Per Item";

                // String for hamburger price
                sellOptionStringsOSS << "$ " << std::fixed << std::setprecision(2) << HAMBURGER_PRICE;
                std::string hamburgerPriceString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chiliburger price
                sellOptionStringsOSS << "$ " << CHILIBURGER_PRICE;
                std::string chiliburgerPriceString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for hotdog price
                sellOptionStringsOSS << "$ " << HOTDOG_PRICE;
                std::string hotdogPriceString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chilidog price
                sellOptionStringsOSS << "$ " << CHILIDOG_PRICE;
                std::string chilidogPriceString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // String for chili
                sellOptionStringsOSS << "$ " << CHILI_SELF_PRICE;
                std::string chiliSelfPriceString = sellOptionStringsOSS.str();
                sellOptionStringsOSS.str("");
                sellOptionStringsOSS.clear();

                // Vector of strings for sell options to use to determine dynamic padding
                std::vector<std::string> sellNumberColumn            = { "#", "0", "1", "2", "3", "4", "5" };
                std::vector<std::string> sellItemOptionColumn        = { itemOptionHeading, hamburgerString, chiliburgerString, hotdogString, chilidogString, chiliSelfString, returnString };
                std::vector<std::string> sellQuantityAvailableColumn = { quantityAvailableHeading, maxQuantityOfHamburgersToSellString, maxQuantityOfChiliburgersToSellString, maxQuantityOfHotdogsToSellString, maxQuantityOfChilidogsToSellString, maxQuantityOfChiliSelfToSellString };
                std::vector<std::string> sellCostPerItemColumn       = { costPerItemHeading, hamburgerPriceString, chiliburgerPriceString, hotdogPriceString, chilidogPriceString, chiliSelfPriceString };

                // Length of longest string of each sell option column
                const int LONGEST_SELL_NUMBER_LENGTH             = getLongestStringLength(sellNumberColumn);
                const int LONGEST_SELL_ITEM_OPTION_LENGTH        = getLongestStringLength(sellItemOptionColumn);
                const int LONGEST_SELL_QUANTITY_AVAILABLE_LENGTH = getLongestStringLength(sellQuantityAvailableColumn);
                const int LONGEST_SELL_COST_PER_ITEM_LENGTH      = getLongestStringLength(sellCostPerItemColumn);

                // Lengths of each width for padding
                const int SELL_NUMBER_WIDTH             = LONGEST_SELL_NUMBER_LENGTH;
                const int SELL_ITEM_OPTION_WIDTH        = LONGEST_SELL_ITEM_OPTION_LENGTH + 4;
                const int SELL_QUANTITY_AVAILABLE_WIDTH = LONGEST_SELL_QUANTITY_AVAILABLE_LENGTH + 4;
                const int SELL_COST_PER_ITEM_WIDTH      = LONGEST_SELL_COST_PER_ITEM_LENGTH + 4;

                // Print formatted table.
                std::stringstream sellOptionOSS;
                sellOptionOSS << std::endl;
                for (unsigned int i = 0; i < sellNumberColumn.size() - 1; ++i) {
                    sellOptionOSS << std::left << std::setw(SELL_NUMBER_WIDTH) << sellNumberColumn.at(i) << std::right << std::setw(SELL_ITEM_OPTION_WIDTH) << sellItemOptionColumn.at(i) << std::setw(SELL_QUANTITY_AVAILABLE_WIDTH) << sellQuantityAvailableColumn.at(i) << std::setw(SELL_COST_PER_ITEM_WIDTH) << sellCostPerItemColumn.at(i) << std::endl;
                }
                sellOptionOSS << std::left << std::setw(SELL_NUMBER_WIDTH) << sellNumberColumn.back() << std::right << std::setw(SELL_ITEM_OPTION_WIDTH) << sellItemOptionColumn.back() << std::endl << std::endl;
                std::cout << sellOptionOSS.str();

                // Execute until valid integer is parsed.
                do {
                    // Prompt user for sell option selection.
                    std::cout << "Enter option for customer order: " << std::flush;

                    // Get string input.
                    std::getline(std::cin, stringInput);

                    // Validate input.
                    sellOptionSelection = getValidInteger(stringInput, std::stoi(sellNumberColumn.at(1)), std::stoi(sellNumberColumn.back()));
                } while (sellOptionSelection == -1);

                // Determine item selected.
                if (sellOptionSelection == 0 && isThereHamburger) { // Hamburger
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for quantity amount.
                        std::cout << std::endl << "Enter quantity (max " << maxQuantityOfHamburgersToSell << "): " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        quantityOfHamburgersToSell = getValidInteger(stringInput, EMPTY_INVENTORY, maxQuantityOfHamburgersToSell, 6);
                    } while (quantityOfHamburgersToSell == -1);

                    // Decrement each ingredient's inventory with quantity ordered. Display warning upon meeting low inventory threshold.
                    currentHamburgerPattyInventory -= quantityOfHamburgersToSell;
                    if (currentHamburgerPattyInventory <= HAMBURGER_PATTY_LOW_INVENTORY) {
                        std::cout << "Warning: Hamburger patty inventory low. Please restock soon." << std::endl;
                    }

                    currentHamburgerBunInventory -= quantityOfHamburgersToSell;
                    if (currentHamburgerBunInventory <= HAMBURGER_BUN_LOW_INVENTORY) {
                        std::cout << "Warning: Hamburger bun inventory low. Please restock soon." << std::endl;
                    }

                    // Calculate item total cost.
                    costOfItemsSold = quantityOfHamburgersToSell * HAMBURGER_PRICE;

                    // Increment order subtotal with item total cost.
                    orderSubtotal += costOfItemsSold;
                } else if (sellOptionSelection == 1 && isThereChiliburger) { // Chiliburger
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for quantity amount.
                        std::cout << std::endl << "Enter quantity (max " << maxQuantityOfChiliburgersToSell << "): " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        quantityOfChiliburgersToSell = getValidInteger(stringInput, EMPTY_INVENTORY, maxQuantityOfChiliburgersToSell, 7);
                    } while (quantityOfChiliburgersToSell == -1);

                    // Decrement each ingredient's inventory with quantity ordered. Display warning upon meeting low inventory threshold.
                    currentHamburgerPattyInventory -= quantityOfChiliburgersToSell;
                    if (currentHamburgerPattyInventory <= HAMBURGER_PATTY_LOW_INVENTORY) {
                        std::cout << "Warning: Hamburger patty inventory low. Please restock soon." << std::endl;
                    }

                    currentHamburgerBunInventory -= quantityOfChiliburgersToSell;
                    if (currentHamburgerBunInventory <= HAMBURGER_BUN_LOW_INVENTORY) {
                        std::cout << "Warning: Hamburger bun inventory low. Please restock soon." << std::endl;
                    }

                    currentChiliInventory -= quantityOfChiliburgersToSell * CHILI_ADDON_SERVING;
                    if (currentChiliInventory <= CHILI_LOW_INVENTORY) {
                        std::cout << "Warning: Chili inventory low. Please restock soon." << std::endl;
                    }

                    // Calculate quantity of each chili serving type available.
                    currentChiliSelfServingsAvailable  = currentChiliInventory / CHILI_SELF_SERVING;
                    currentChiliAddonServingsAvailable = currentChiliInventory / CHILI_ADDON_SERVING;

                    // Calculate item total cost.
                    costOfItemsSold = quantityOfChiliburgersToSell * CHILIBURGER_PRICE;

                    // Increment order subtotal with item total cost.
                    orderSubtotal += costOfItemsSold;
                } else if (sellOptionSelection == 2 && isThereHotdog) { // Hotdog
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for quantity amount.
                        std::cout << std::endl << "Enter quantity (max " << maxQuantityOfHotdogsToSell << "): " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        quantityOfHotdogsToSell = getValidInteger(stringInput, EMPTY_INVENTORY, maxQuantityOfHotdogsToSell, 8);
                    } while (quantityOfHotdogsToSell == -1);

                    // Decrement each ingredient's inventory with quantity ordered. Display warning upon meeting low inventory threshold.
                    currentHotdogInventory -= quantityOfHotdogsToSell;
                    if (currentHotdogInventory <= HOTDOG_LOW_INVENTORY) {
                        std::cout << "Warning: Hotdog inventory low. Please restock soon." << std::endl;
                    }

                    currentHotdogBunInventory -= quantityOfHotdogsToSell;
                    if (currentHotdogBunInventory <= HOTDOG_BUN_LOW_INVENTORY) {
                        std::cout << "Warning: Hotdog bun inventory low. Please restock soon." << std::endl;
                    }

                    // Calculate item total cost.
                    costOfItemsSold = quantityOfHotdogsToSell * HOTDOG_PRICE;

                    // Increment order subtotal with item total cost.
                    orderSubtotal += costOfItemsSold;
                } else if (sellOptionSelection == 3 && isThereChilidog) { // Chilidog
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for quantity amount.
                        std::cout << std::endl << "Enter quantity (max " << maxQuantityOfChilidogsToSell << "): " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        quantityOfChilidogsToSell = getValidInteger(stringInput, EMPTY_INVENTORY, maxQuantityOfChilidogsToSell, 9);
                    } while (quantityOfChilidogsToSell == -1);

                    // Decrement each ingredient's inventory with quantity ordered. Display warning upon meeting low inventory threshold.
                    currentHotdogInventory -= quantityOfChilidogsToSell;
                    if (currentHotdogInventory <= HOTDOG_LOW_INVENTORY) {
                        std::cout << "Warning: Hotdog inventory low. Please restock soon." << std::endl;
                    }

                    currentHotdogBunInventory -= quantityOfChilidogsToSell;
                    if (currentHotdogBunInventory <= HOTDOG_BUN_LOW_INVENTORY) {
                        std::cout << "Warning: Hotdog bun inventory low. Please restock soon." << std::endl;
                    }

                    currentChiliInventory -= quantityOfChilidogsToSell * CHILI_ADDON_SERVING;
                    if (currentChiliInventory <= CHILI_LOW_INVENTORY) {
                        std::cout << "Warning: Chili inventory low. Please restock soon." << std::endl;
                    }

                    // Calculate quantity of each chili serving type available.
                    currentChiliSelfServingsAvailable  = currentChiliInventory / CHILI_SELF_SERVING;
                    currentChiliAddonServingsAvailable = currentChiliInventory / CHILI_ADDON_SERVING;

                    // Calculate item total cost.
                    costOfItemsSold = quantityOfChilidogsToSell * CHILIDOG_PRICE;

                    // Increment order subtotal with item total cost.
                    orderSubtotal += costOfItemsSold;
                } else if (sellOptionSelection == 4 && isThereChiliSelf) { // Chili
                    // Execute until valid integer is parsed.
                    do {
                        // Prompt for quantity amount.
                        std::cout << std::endl << "Enter quantity (max " << maxQuantityOfChiliSelfToSell << "): " << std::flush;

                        // Get string input.
                        std::getline(std::cin, stringInput);

                        // Validate input.
                        quantityOfChiliSelfToSell = getValidInteger(stringInput, EMPTY_INVENTORY, maxQuantityOfChiliSelfToSell, 10);
                    } while (quantityOfChiliSelfToSell == -1);

                    // Decrement each ingredient's inventory with quantity ordered. Display warning upon meeting low inventory threshold.
                    currentChiliInventory -= quantityOfChiliSelfToSell * CHILI_SELF_SERVING;
                    if (currentChiliInventory <= CHILI_LOW_INVENTORY) {
                        std::cout << "Warning: Chili inventory low. Please restock soon." << std::endl;
                    }

                    // Calculate quantity of each chili serving type available.
                    currentChiliSelfServingsAvailable  = currentChiliInventory / CHILI_SELF_SERVING;
                    currentChiliAddonServingsAvailable = currentChiliInventory / CHILI_ADDON_SERVING;

                    // Calculate item total cost.
                    costOfItemsSold = quantityOfChiliSelfToSell * CHILI_SELF_PRICE;

                    // Increment order subtotal with item total cost.
                    orderSubtotal += costOfItemsSold;
                } else if (sellOptionSelection == 5) {
                    // Calculate tax total.
                    orderTax= orderSubtotal * SALES_TAX;
                    // Calculate order total.
                    orderTotal = orderSubtotal + orderTax;

                    // Print order total.
                    std::stringstream orderTotalOSS;
                    orderTotalOSS << std::endl << "Order Total: $ " << std::fixed << std::setprecision(2) << orderTotal << std::endl;
                    std::cout << orderTotalOSS.str();

                    // Exit loop.
                    break;
                } else {
                    // Print message indicating there is a lack of stock for the item.
                    std::cout << std::endl << "Invalid input, please enter an item with quantity available or update inventory." << std::endl;
                }
            } while (sellOptionSelection != 5);
        }
    }

    // Exit program successfully.
    return 0;
}

/**
 * @brief getLongestStringLength loops through each of the strings of a vector, finds the longest string, and returns the string length to use for setting field widths.
 * @param columnStrings = Column strings vector constant passed by reference
 * @return = Integer with string length result.
 */

int getLongestStringLength(const std::vector<std::string>& columnStrings) {
    // Stores first string.
    std::string currentColumnString = columnStrings.at(0);

    // Initializes current string length integer for for loop.
    int currentColumnStringLength = 0;
    // Stores first string length to longest string length integer.
    int longestColumnStringLength = currentColumnString.length();

    // Loops through rest of strings to determine longest string length.
    for (unsigned int i = 1; i < columnStrings.size(); ++i) {
        currentColumnString = columnStrings.at(i);
        currentColumnStringLength = currentColumnString.length();

        if (currentColumnStringLength > longestColumnStringLength) {
            longestColumnStringLength = currentColumnStringLength;
        }
    }

    return longestColumnStringLength;
}

/**
 * @brief stringToIntegerValidation parses an integer from a string and returns the error result.
 * @param parsedInteger = Integer to receive parsed result passed by reference
 * @param stringInputPointer = Passed pointer to null-terminated byte string constant to be interpreted
 * @param base = Integer to determine base of integer passed by value (default to 0 for auto-detected base)
 * @return = stringToIntegerError enum representing the validation result from parsing string to integer
 */

stringToIntegerError stringToIntegerValidation (int &parsedInteger, const char *stringInputPointer, int base) {
    // Error result to return
    stringToIntegerError errorResult;
    // Pointer to pointer of character past the last character interpreted
    char *end;
    // Long to parse from string and to be cast to integer after validation
    long longToParse;
    // A preprocesor macro initialized to 0 for upcoming validation. In this case, it is used to determine if the long receives a value that is out of range.
    errno = 0;
    // Interpret integer value from byte string pointed to by the stringInputPointer.
    longToParse = std::strtol(stringInputPointer, &end, base);

    // Determine if errno is out of range AND long returns LONG_MAX (the maximum value of a long) OR long exceeds INT_MAX (the maximum value of an integer). Upon error, long returns 0.
    if ((errno == ERANGE && longToParse == LONG_MAX) || longToParse > INT_MAX) {
        errorResult = STRTOINT_OVERFLOW;
    } else if ((errno == ERANGE && longToParse == LONG_MIN) || longToParse < INT_MIN) { // Determine if errno is out of range AND long returns LONG_MIN (the minimum value of a long) OR long exceeds INT_MIN (the minimum value of an integer). Upon error, long returns 0.
        errorResult = STRTOINT_UNDERFLOW;
    } else if (*stringInputPointer == '\0' || *end != '\0') { // Determine if byte string or character pointer of last pointer returns null. Prevents input such as "5g".
        errorResult = STRTOINT_INCONVERTIBLE;
    } else { // Input is a valid integer.
        errorResult = STRTOINT_SUCCESS;
        // Cast valid long to integer.
        parsedInteger = static_cast<int>(longToParse);
    }

    // Return error result.
    return errorResult;
}

/**
 * @brief stringToInteger takes a given string and attempts to parse and return an integer. Upon error, integer is -1.
 * @param stringInput = Input string to be parsed
 * @param minValue = Minimum valid integer value
 * @param maxValue = Maximum valid integer value
 * @param messageType = Message type for exceeding minimum or maximum valid integer value
 * @return = an integer parsed from the input string
 */

int getValidInteger(std::string stringInput, int minValue, int maxValue, int messageType) {
    // Convert string to c-string and then convert to constant byte string to pass to validation function.
    const char * stringInputPointer = stringInput.c_str();

    // Declare integer to pass by reference and to store parsed result from input string.
    int integerFromString = -1;

    // Call validation function and store error result to enum variable.
    stringToIntegerError errorResult = stringToIntegerValidation(integerFromString, stringInputPointer);

    // String streams for message type
    std::stringstream exceedMaxValueOSS;
    std::stringstream exceedMinValueOSS;

    // Get exceed minimum and maximum value strings for upcoming error check.
    if (messageType == 0) { // Default
        exceedMaxValueOSS << "Input is too high. Please enter an integer between " << minValue << " and " << maxValue << ".";
        exceedMinValueOSS << "Input is too low. Please enter an integer between " << minValue << " and " << maxValue << ".";
    } else if (messageType ==  1) { // Hamburger Patty Inventory
        exceedMaxValueOSS << "Exceeded max hamburger patty capacity (" << maxValue << "). Please enter a valid inventory.";
        exceedMinValueOSS << "Invalid input. Please enter a valid inventory.";
    } else if (messageType ==  2) { // Hamburger Bun Inventory
        exceedMaxValueOSS << "Exceeded max hamburger bun capacity (" << maxValue << "). Please enter a valid inventory.";
        exceedMinValueOSS << "Invalid input. Please enter a valid inventory.";
    } else if (messageType ==  3) { // Hotdog Inventory
        exceedMaxValueOSS << "Exceeded max hotdog capacity (" << maxValue << "). Please enter a valid inventory.";
        exceedMinValueOSS << "Invalid input. Please enter a valid inventory.";
    } else if (messageType ==  4) { // Hotdog Bun Inventory
        exceedMaxValueOSS << "Exceeded max hotdog bun capacity (" << maxValue << "). Please enter a valid inventory.";
        exceedMinValueOSS << "Invalid input. Please enter a valid inventory.";
    } else if (messageType ==  5) { // Chili Inventory
        exceedMaxValueOSS << "Exceeded max chili capacity (" << maxValue << "). Please enter a valid inventory.";
        exceedMinValueOSS << "Invalid input. Please enter a valid inventory.";
    } else if (messageType ==  6) { // Hamburger Quantity
        exceedMaxValueOSS << "Exceeded quantity of hamburgers available (" << maxValue << "). Please enter a valid quantity.";
        exceedMinValueOSS << "Invalid input. Please enter a valid quantity.";
    } else if (messageType ==  7) { // Chiliburger Quantity
        exceedMaxValueOSS << "Exceeded quantity of chiliburgers available (" << maxValue << "). Please enter a valid quantity.";
        exceedMinValueOSS << "Invalid input. Please enter a valid quantity.";
    } else if (messageType ==  8) { // Hotdog Quantity
        exceedMaxValueOSS << "Exceeded quantity of hotdogs available (" << maxValue << "). Please enter a valid quantity.";
        exceedMinValueOSS << "Invalid input. Please enter a valid quantity.";
    } else if (messageType ==  9) { // Chilidog Quantity
        exceedMaxValueOSS << "Exceeded quantity of chilidogs available (" << maxValue << "). Please enter a valid quantity.";
        exceedMinValueOSS << "Invalid input. Please enter a valid quantity.";
    } else if (messageType == 10) { // Chili Quantity
        exceedMaxValueOSS << "Exceeded quantity of chili available (" << maxValue << "). Please enter a valid quantity.";
        exceedMinValueOSS << "Invalid input. Please enter a valid quantity.";
    }

    // Determine error result.
    if (errorResult == STRTOINT_OVERFLOW) {
        // Print message informing user that input is too high (e.g. "99999999999999999999999999999999999999").
        std::cout << "Input is too high. Please enter an integer between " << minValue << " and " << maxValue << "." << std::endl;
    } else if (errorResult == STRTOINT_UNDERFLOW) {
        // Print message informing user that input is too low (e.g. "-11111111111111111111111111111111111111").
        std::cout << "Input is too low. Please enter an integer between " << minValue << " and " << maxValue << "." << std::endl;
    } else if (errorResult == STRTOINT_INCONVERTIBLE) {
        // Print message informing user that input is not a valid integer (e.g., "5g", "-5g", "9 9").
        std::cout << "Invalid input. Please enter an integer." << std::endl;
    } else if (integerFromString > maxValue) {
        // Print message informing user that input is too high (e.g. maxValue + 1).
        std::cout << exceedMaxValueOSS.str() << std::endl;
        // Reassign parsed integer to -1.
        integerFromString = -1;
    } else if (integerFromString < minValue) {
        // Print message informing user that input is too low (e.g. minValue - 1).
        std::cout << exceedMinValueOSS.str() << std::endl;
        // Reassign parsed integer to -1.
        integerFromString = -1;
    }

    // Returns parsed integer.
    return integerFromString;
}
