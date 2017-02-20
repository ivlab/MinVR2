#include <iostream>
#include <stdlib.h>
#include "config/VRDataIndex.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif


bool fileGood(const std::string &fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

std::string findDataFile(const std::string &filename) {
    std::string fname;
    std::vector< std::string > checked;
    
    // 1. current working directory
    fname = "./" + filename;
    checked.push_back(fname);
    if (fileGood(fname)) {
        return fname;
    }
        
    // 2. an installed version based on the INSTALL_PREFIX set with cmake
    fname = std::string(INSTALLPATH) + "/share/memtest/" + filename;
    checked.push_back(fname);
    if (fileGood(fname)) {
        return fname;
    }
    
    // 3. an installed version based on MINVR_ROOT envvar
    if (std::getenv("MINVR_ROOT")) {
        fname = std::string(std::getenv("MINVR_ROOT")) + "/share/memtest/" + filename;
        checked.push_back(fname);
        if (fileGood(fname)) {
          return fname;
        }
    }

    std::cerr << "memtest Error: Cannot find data file " << filename
              << " in the following " << checked.size() << " locations:" << std::endl;
    for (int i=0; i<checked.size(); i++) {
        std::cerr << i+1 << ": " << checked[i] << std::endl;
    }
    throw std::runtime_error("memtest Error: Cannot find data file " + filename);
}



int main(int argc, char** argv) {

  MinVR::VRDataIndex *n = new MinVR::VRDataIndex;

  struct malloc_introspection_t	*in;
  
  MinVR::VRInt a = 4;
  MinVR::VRFloat b = 3.1415926;
  
  n->addData("/george/a0", a);
  n->addData("/george/a1", a + 1);
  n->addData("/george/a2", a + 2);
  n->addData("/george/a3", a + 3);
  n->addData("/george/a4", a + 4);
  n->addData("/george/a5", a + 5);
  n->addData("/george/a6", a + 6);
  n->addData("/george/a7", a + 7);
  n->addData("/george/a8", a + 8);
  n->addData("/george/a9", a + 9);

  n->addData("/martha/b0", b);
  n->addData("/martha/b1", b * 1);
  n->addData("/martha/b2", b * 2);
  n->addData("/martha/b3", b * 3);
  n->addData("/martha/b4", b * 4);
  n->addData("/martha/b5", b * 5);
  n->addData("/martha/b6", b * 6);
  n->addData("/martha/b7", b * 7);
  n->addData("/martha/b8", b * 8);
  n->addData("/martha/b9", b * 9);

  MinVR::VRString c = "abigail";
  n->addData("/john/c0", c + "0");
  n->addData("/john/c1", c + "1");
  n->addData("/john/c2", c + "2");
  n->addData("/john/c3", c + "3");
  n->addData("/john/c4", c + "4");
  n->addData("/john/c5", c + "5");
  n->addData("/john/c6", c + "6");
  n->addData("/john/c7", c + "7");
  n->addData("/john/c8", c + "8");
  n->addData("/john/c9", c + "9");

  std::vector<float>d;
  d.push_back(1.2);
  d.push_back(2.3);
  d.push_back(3.4);
  d.push_back(4.5);
  d.push_back(5.6);

  n->addData("/donna/d0", d);
  
  std::string filename = findDataFile("test.xml");
  n->processXMLFile(filename, "/");
  
  n->printStructure();
  
  std::cout << "all done: " << std::endl;

  delete n;
}
