#include <iostream> 
#include <unordered_set>
#include <unordered_map>
#include <queue>  //libraries needed for algorithms


//LRU Algorithm
// Function to find page faults using indexes 
int pageFaults(int pages[], int n, int capacity)
{
   
    std::unordered_set<int> s;  // check if a page is present in set or not  
    std::unordered_map<int, int> indexes; // To store least recently used indexes of pages

    int page_faults = 0;  // Start from initial page 
    for (int i = 0; i < n; i++) //loop to check each page 
    {
        // Check if the set can hold more pages 
        if (s.size() < capacity)
        {
            // Insert page into set if not present which indicates page fault
            if (s.find(pages[i]) == s.end())
            {
                s.insert(pages[i]);

                // increment page fault 
                page_faults++;
            }

            indexes[pages[i]] = i; // Store the recently used index of each page 
        }

        // If the set is full then need to perform lru i.e. remove the least recently used page and insert the current page 
        else
        {
            // Check if current page is not already present in the set 
            if (s.find(pages[i]) == s.end())
            {
                // Find the least recently used pages that is present in the set 
                int lru = INT_MAX, val;
                for (auto it = s.begin(); it != s.end(); it++)
                {
                    if (indexes[*it] < lru)
                    {
                        lru = indexes[*it];
                        val = *it;
                    }
                }

                // Remove the indexes page 
                s.erase(val);

                // insert the current page 
                s.insert(pages[i]);

                // Increment page faults 
                page_faults++;
            }

            // Update the current page index 
            indexes[pages[i]] = i;
        }
        std::cout << "Step " << i + 1 << ": Page fault (" << pages[i] << ") - Page Table: "; 
        for (auto& entry : s) {
            std::cout << "{" << entry << "}, ";
        }
        std::cout << "Faults: " << page_faults << std::endl;   //display each step for the reference string with the page fault 
    }

    return page_faults;  //return the amount
}



// Function to check whether a page exists in a frame or not
bool search(int key, std::vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}

// Function to find the frame that will not be used recently in future after given index in pg[0..pn-1]
int predict(int pg[], std::vector<int>& fr, int pn, int index)
{
    // Store the index of pages which are going to be used recently in future
    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }

        // If a page is never referenced in future, return it.
        if (j == pn)
            return i;
    }

    // If all of the frames were not in future, return any of them, we return 0. Otherwise we return res.
    return (res == -1) ? 0 : res;
}

//optimal alogrithm
void optimalPage(int pg[], int pn, int fn)
{
    std::vector<int> fr; // Create an array for given number of frames
    int fault_count = 0;
    for (int i = 0; i < pn; i++) {
        if (!search(pg[i], fr)) { // If page not found in frames
            fault_count++; // Increment fault count
            if (fr.size() < fn)
                fr.push_back(pg[i]); // If frames are not full, add the page
            else {
                int j = predict(pg, fr, pn, i + 1); // Find the page to be replaced
                fr[j] = pg[i]; // Replace the page
            }
        }
        std::cout << "Step " << i + 1 << ": Page fault (" << pg[i] << ") - Page Table: ";
        for (auto& entry : fr) {
            std::cout << "{" << entry << "}, ";
        }
        std::cout << "Frames: [";
        for (int frame : fr) {
            std::cout << frame << ", ";
        }
        std::cout << "], Faults: " << fault_count << std::endl; //display the fault and frame each step of the way for given input
    }
    std::cout << "Total Page Faults for Optimal: " << fault_count << std::endl << std::endl;  //display total page faults
}


//FIFO algorithm
int pageFaultss(int pages[], int n, int capacity)
{

    std::unordered_set<int> s; // To represent set of current pages. We use an unordered_set so that we quickly check if a page is present in set or not 
    std::queue<int> indexes; // To store the pages in FIFO manner 

    // Start from initial page 
    int page_faults = 0;
    for (int i = 0; i < n; i++)
    {
        // Check if the set can hold more pages 
        if (s.size() < capacity)
        {
            // Insert it into set if not present already which represents page fault 
            if (s.find(pages[i]) == s.end())
            {
                // Insert the current page into the set 
                s.insert(pages[i]);

                // increment page fault 
                page_faults++;

                // Push the current page into the queue 
                indexes.push(pages[i]);
            }
        }

        // If the set is full then need to perform FIFO i.e. remove the first page of the queue from set and queue both and insert the current page 
        else
        {
            // Check if current page is not already present in the set 
            if (s.find(pages[i]) == s.end())
            {
                // Store the first page in the queue to be used to find and erase the page from the set 
                int val = indexes.front();
                // Pop the first page from the queue 
                indexes.pop();
                // Remove the indexes page from the set 
                s.erase(val);
                // insert the current page in the set 
                s.insert(pages[i]);
                // push the current page into the queue 
                indexes.push(pages[i]);
                // Increment page faults 
                page_faults++;
            }
        }
        std::cout << "Step " << i + 1 << ": Page fault (" << pages[i] << ") - Page Table: ";
        for (auto& entry : s) {
            std::cout << "{" << entry << "}, ";
        }
        std::cout << "Faults: " << page_faults << std::endl;   //each step of page fault and display that
    }

    return page_faults;
}


int main()
{
    int pages[13];// = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };    test case
    int capacity;// = 4;
    std::cout << "Press the ENTER key after each number. Enter your page reference string equivalent to 13 numbers: " <<std::endl;
    for (int i = 0; i < 13; i++)
    {
        std::cin >> pages[i];
        if(pages[i] < 0)    //ensure there is not negative number, if there is, ask user for new number
        {
            std::cerr << "Error. Enter a positive number: ";
            std::cin >> pages[i];
        }
    }  //ask for a reference string, have user input with enter after each number, ensure number is positive
    int n = sizeof(pages) / sizeof(pages[0]);
    std::cout << "Enter the number of frames: ";
    std::cin >> capacity;
    while (capacity <= 0) 
    {
        std::cerr << "Error. Enter a positive number greater than 0: ";
        std::cin >> capacity;       //ask for capacity and ensure it is positive
    }           

    std::cout << "Total Page Faults for LRU: " << pageFaults(pages, n, capacity) << std::endl << std::endl;

    optimalPage(pages, n, capacity);

    std::cout << "Total Page Faults for FIFO: " << pageFaultss(pages, n, capacity) <<std::endl <<std::endl;  //display the result for each of the algorithms

    return 0;
}