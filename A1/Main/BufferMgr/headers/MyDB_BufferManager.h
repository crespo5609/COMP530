
#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "MyDB_PageHandle.h"
#include "MyDB_Table.h"
#include "MyDB_Page.h"
#include "MyDB_LRUTable.h"
#include <memory>

#include <vector>
#include <unordered_map>

using namespace std;

class MyDB_BufferManager;
typedef shared_ptr<MyDB_BufferManager> MyDB_BufferManagerPtr ;
typedef unordered_map<string, MyDB_PagePtr> id_to_PagePtr_map;


class MyDB_BufferManager : public enable_shared_from_this<MyDB_BufferManager> {
    
public:
    
    // THESE METHODS MUST APPEAR AND THE PROTOTYPES CANNOT CHANGE!
    
    // gets the i^th page in the table whichTable... note that if the page
    // is currently being used (that is, the page is current buffered) a handle
    // to that already-buffered page should be returned
    MyDB_PageHandle getPage (MyDB_TablePtr whichTable, long i);
    
    // gets a temporary page that will no longer exist (1) after the buffer manager
    // has been destroyed, or (2) there are no more references to it anywhere in the
    // program.  Typically such a temporary page will be used as buffer memory.
    // since it is just a temp page, it is not associated with any particular
    // table
    MyDB_PageHandle getPage ();
    
    // gets the i^th page in the table whichTable... the only difference
    // between this method and getPage (whicTable, i) is that the page will be
    // pinned in RAM; it cannot be written out to the file
    MyDB_PageHandle getPinnedPage (MyDB_TablePtr whichTable, long i);
    
    // gets a temporary page, like getPage (), except that this one is pinned
    MyDB_PageHandle getPinnedPage ();
    
    // un-pins the specified page
    void unpin (MyDB_PageHandle unpinMe);
    
    // creates an LRU buffer manager... params are as follows:
    // 1) the size of each page is pageSize
    // 2) the number of pages managed by the buffer manager is numPages;
    // 3) temporary pages are written to the file tempFile
    MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile);
    
    // when the buffer manager is destroyed, all of the dirty pages need to be
    // written back to disk, any necessary data needs to be written to the catalog,
    // and any temporary files need to be deleted
    ~MyDB_BufferManager ();
    
    char* allocBuffer (MyDB_PagePtr pagePtr);
    
    
private:
    
    // YOUR STUFF HERE
    size_t _pageSize;
    size_t _numPages;
    string _tempFile;
    vector<char*> pageFrames;
    vector<int> availableSlots;
    id_to_PagePtr_map _pageTable;
    MyDB_LRUTablePtr _lruTable;
    
};

#endif
