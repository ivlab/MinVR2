#include "VRDataQueue.h"

// Use this when the client has no new data to offer.
const VRDataQueue::serialData VRDataQueue::noData = "";


VRDataQueue::VRDataQueue(const VRDataQueue::serialData serializedQueue) {

  addSerializedQueue(serializedQueue);
  
}

// This function does *not* process arbitrary XML, but it *does*
// process XML that was produced by the serialize() method below.
// This is why it looks a bit hacky, with mysterious constants like
// the number 18.  This serialization is only intended to transmit
// from one instance of this class to another, even if it more or less
// honors the look and feel of XML.
void VRDataQueue::addSerializedQueue(const VRDataQueue::serialData serializedQueue) {
  
  // Looking for the number in <VRDataQueue num="X">
  if (serializedQueue.size() < 18) return;
  
  std::size_t start, end;
  
  start = 18;
  end = serializedQueue.find("\"", start);
  
  //std::cout << serializedQueue.substr(start, end - start) << std::endl;
  
  int numIncluded;
  std::istringstream( serializedQueue.substr(start,end)) >> numIncluded;
  start = serializedQueue.find(">", end);

  // Found the number of items.  Now parse the items, making sure to
  // record the timeStamp of each one.

  int numReceived = 0;
  long long timeStamp;
  start = serializedQueue.find("<VRDataQueueItem timeStamp=\"", start);
  while (start != std::string::npos) {

    end = serializedQueue.find("\">", start);
    // These constants have to do with the length of the tags used to
    // encode a queue.
    std::istringstream(serializedQueue.substr(start + 28, end - start - 28)) >>
      timeStamp;

    start = serializedQueue.find("</VRDataQueueItem>", end);
    push(timeStamp, serializedQueue.substr(end + 2, start - end - 2) );
    numReceived++;
    start = serializedQueue.find("<VRDataQueueItem timeStamp=\"", end);
  }

  if (numReceived != numIncluded) {
    throw std::runtime_error(std::string("serialized queue appears corrupted"));
  }
}

VRDataQueue::serialData VRDataQueue::getSerializedObject() {
  if (dataMap.empty()) {
      
    return "";
  } else {

    return dataMap.begin()->second;
  }
}

void VRDataQueue::pop() {
  dataMap.erase(dataMap.begin());
}

void VRDataQueue::clear() {
  dataMap.clear();
}

void VRDataQueue::push(const VRDataQueue::serialData serializedData) {

#ifdef WIN32
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1;
	QueryPerformanceFrequency(&frequency);

	// start timer
	QueryPerformanceCounter(&t1);

	long long timeStamp = (t1.QuadPart) * 1000.0 / frequency.QuadPart + clock();;
#else

  struct timeval tp;
  gettimeofday(&tp, NULL);
  
  // Get current timestamp in milliseconds.
  long long timeStamp = (long long) tp.tv_sec * 1000000L + tp.tv_usec;

#endif

  push(timeStamp, serializedData);
}

void VRDataQueue::push(const long long timeStamp,
                       const VRDataQueue::serialData serializedData) {

  dataMap.insert(std::pair<long long,VRDataQueue::serialData>(timeStamp, serializedData));
}


VRDataQueue::serialData VRDataQueue::serialize() {

  std::ostringstream lenStr;
  lenStr << dataMap.size();

  VRDataQueue::serialData out;

  out = "<VRDataQueue num=\"" + lenStr.str() + "\">";
  for (VRDataList::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
    std::ostringstream timeStamp;
    timeStamp << it->first;
    out += "<VRDataQueueItem timeStamp=\"" + timeStamp.str() + "\">" +
      it->second + "</VRDataQueueItem>";
  }
  out += "</VRDataQueue>";

  return out;
}

// DEBUG only
std::string VRDataQueue::printQueue() {

  std::string out;

  char buf[3];
  int i = 0;
  for (VRDataList::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
    sprintf(buf, "%d", ++i);
    out += "element " + std::string(buf) + ": " + it->second + "\n";
  }

  return out;
}
