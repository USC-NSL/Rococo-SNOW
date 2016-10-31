#pragma once

#include "../__dep__.h"
#include "../coordinator.h"
#include "../frame.h"

namespace rococo {

class MultiPaxosCommo;
class MultiPaxosCoord : public Coordinator {
 public:
//  static ballot_t next_slot_s;
 private:
  MultiPaxosCommo* commo() {
    // TODO fix this.
    verify(commo_ != nullptr);
    return (MultiPaxosCommo*) commo_;
  }
  bool in_submission_ = false; // debug;
  bool in_prepare_ = false; // debug
  bool in_accept = false; // debug
 public:
  ContainerCommand* cmd_ = nullptr;
//  using Coordinator::Coordinator;
  MultiPaxosCoord(uint32_t coo_id,
                  int32_t benchmark,
                  ClientControlServiceImpl *ccsi,
                  uint32_t thread_id);
  ballot_t curr_ballot_ = 0; // TODO
  uint32_t n_replica_ = 0;   // TODO
  slotid_t slot_id_ = 0;
  ballot_t *slot_hint_ = nullptr;

  uint32_t n_replica() {
    verify(n_replica_ > 0);
    return n_replica_;
  }

  slotid_t GetNextSlot() {
    verify(0);
    verify(slot_hint_ != nullptr);
    slot_id_ = (*slot_hint_)++;
    return 0;
  }

  uint32_t GetQuorum() {
//    return n_replica(); // TODO
    return n_replica() / 2 + 1;
  }

  void do_one(TxnRequest &req) override {}
  void Submit(ContainerCommand& cmd,
              const std::function<void()>& func = [] () {},
              const std::function<void()>& exe_callback = [] () {}) override;

  ballot_t PickBallot();
  void Prepare();
  void PrepareAck(phase_t phase, Future *fu);
  void Accept();
  void AcceptAck(phase_t phase, Future *fu);
  void Decide();

  void Reset() override {}
  void Restart() override {verify(0);}
};

} //namespace rococo