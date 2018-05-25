#ifndef NodeID_h
#define NodeID_h

#include <ctype.h>

class NodeID {
  public: 

  uint8_t val[6];
  
  NodeID() {
      val[0] = 0;
      val[1] = 0;
      val[2] = 0;
      val[3] = 0;
      val[4] = 0;
      val[5] = 0;
  }
  
  NodeID(uint8_t b0, uint8_t b1, uint8_t b2, 
         uint8_t b3, uint8_t b4, uint8_t b5) {
      val[0] = b0;
      val[1] = b1;
      val[2] = b2;
      val[3] = b3;
      val[4] = b4;
      val[5] = b5;
  }
  
  bool equals(NodeID* n) {
    return  (val[0]==n->val[0])&&(val[1]==n->val[1])
          &&(val[2]==n->val[2])&&(val[3]==n->val[3])
          &&(val[4]==n->val[4])&&(val[5]==n->val[5]);
     }

  /**
   * Check to see if this object is equal
   * to any in an array of NodeIDs
   */
  NodeID* findInArray(NodeID* array, int len) {
      for (int i = 0; i<len; i++) {
          if (equals(array+i)) return array+i;
      }
      return 0;
  }

};

#endif
