#ifndef __PHANDLER_H__
#define __PHANDLER_H__

#include <Pixy2.h>


class PixyHandler
{
  private:
    Pixy2 pxy;

  public:
    // constructor
    PixyHandler(Pixy2 pixy)
    {
      pxy = pixy;
    }

    Target simpleGetBlockData(int desiredSignature)
    {
      Target discovered;
      
      // grab blocks!
      pxy.ccc.getBlocks(desiredSignature);

      // let's see if there's more than one instance of what we've been looking for
      for (int i = pxy.ccc.numBlocks; i >= 0; i--)
      {
        // Search through each block  starting with the last seen block first
        // Assuming last seen is last on array
        if (pxy.ccc.blocks[i].m_signature != desiredSignature)
        {
          Serial.print("Something went wrong; got blocks not specified: "); 
          Serial.print(pxy.ccc.blocks[i].m_signature); 
          Serial.print(" vs "); 
          Serial.print(desiredSignature); 
          continue;
        }
        if(pxy.ccc.blocks[i].m_x == 0 && pxy.ccc.blocks[i].m_y == -1)
        {
          //didn't see a block
          Serial.println("x = 0, y = -1");
          continue;
        }
        // let's just take the first instance of our object found, set it, and get outta here!
            discovered.setX(pxy.ccc.blocks[i].m_x);
            discovered.setY(pxy.ccc.blocks[i].m_y);
            break;
      }
      return discovered;
    }

    
    Target getUpdatedBlockData(int desiredSignature, int trackingIndex)
    { 
      Target discovered;
      int recurrence = 0;
      // grab blocks!
      //pxy.ccc.getBlocks(desiredSignature);
      pxy.ccc.getBlocks();

      // let's see if there's more than one instance of what we've been looking for
      for (int i = pxy.ccc.numBlocks; i >= 0; i--)
      {
        // Search through each block  starting with the last seen block first
        // Assuming last seen is last on array
        if (pxy.ccc.blocks[i].m_signature != desiredSignature)
        {
//          Serial.print("Something went wrong; got blocks not specified: "); 
//          Serial.print(pxy.ccc.blocks[i].m_signature); 
//          Serial.print(" vs "); 
//          Serial.println(desiredSignature); 
          continue;
        }

        Serial.println("found desired");

        //Check to see if we've already been tracking this instance
        if (pxy.ccc.blocks[i].m_index == trackingIndex)
        {
          // Same object we'd been tracking was found
          recurrence++;
        }
      }

      int prevAge = 0;
      // Search for the most recent sitting of our desired object
      for (int i = pxy.ccc.numBlocks; i >= 0; i--)
      {
        // Ideally we want to track our same object, use the tracking index to do this
        if (pxy.ccc.blocks[i].m_index == trackingIndex)
        {
          if (recurrence > 1)
          {
            // We found more than one instance of our tracked object, get the one with the greatest age
            if (pxy.ccc.blocks[i].m_age > prevAge)
            {
              discovered.setX(pxy.ccc.blocks[i].m_x);
              discovered.setY(pxy.ccc.blocks[i].m_y);
              continue;
            }
          }
          else
          {
            // only one instance of our object found, set it and get outta here!
            discovered.setX(pxy.ccc.blocks[i].m_x);
            discovered.setY(pxy.ccc.blocks[i].m_y);
            break;
          }
        }
        else
        {
          // We hadn't been tracking this before so let's select the new instance of our object with the oldest age
          if (pxy.ccc.blocks[i].m_age > prevAge)
          {
            discovered.populateTarget(pxy.ccc.blocks[i].m_signature, pxy.ccc.blocks[i].m_index, pxy.ccc.blocks[i].m_x, pxy.ccc.blocks[i].m_y);
            //Serial.print("Tracking new instance of object "); //TODO bring back
            //Serial.println(discovered.getSignature());
            prevAge = pxy.ccc.blocks[i].m_age;
          }
        }
      }
      return discovered;
    }
};
#endif
