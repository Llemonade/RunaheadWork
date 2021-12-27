#ifndef __CPU_DYN_INST_PARENT_HH__
#define __CPU_DYN_INST_PARENT_HH__

#include "mem/request.hh"
#include "base/types.hh"
#include <bitset>
#include "debug/RunaheadEnter.hh"

namespace gem5
{

class DynInstParent {
protected:
  enum Flags
  {
      MissedInL2,
      MaxFlags
  };
  std::bitset<MaxFlags> runaheadFlags;

  /* outstanding requests generated by this instruction
  (i.e. those which haven't returned with data yet).
  This is normally a signgle request, but when trying
  to access data split accross two cache lines,
  this may result in more requests being created */
  std::vector<RequestPtr> _outstandingReqs;

  // 
  bool _runaheadInst = false;

public:
  virtual void setL2Miss()  { runaheadFlags.set(MissedInL2); }
  void resetL2Miss()        { runaheadFlags.reset(MissedInL2); }
  bool missedInL2()         { return runaheadFlags[MissedInL2]; }
  
  virtual bool isExecuted() const = 0;
  virtual bool isSquashed() const = 0;
  virtual int getSeqNum() = 0;
  virtual Addr instAddr() const = 0;
  virtual bool isInROB() const = 0;

  void addReq(RequestPtr req);

  bool numOutstandingRequests() { return _outstandingReqs.size(); }
  void reqCompleted(RequestPtr req);

  bool isRunaheadInst() { return _runaheadInst; }
  void setRunaheadInst();
  // reset does not modify any request associated with this instruction
  void resetRunaheadInst();

};

} // gem5
#endif // __CPU_DYN_INST_PARENT_HH__