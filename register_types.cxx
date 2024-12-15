
#include "register_types.h"

// Godot headers
#include "dvector.h"
#include "object_type_db.h"
#include "reference.h"
#include "ustring.h"

// Wrap for vorpal
#include <vorpal/vorpal.h>

#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace vorpal {
namespace godot {

using std::shared_ptr;
using std::string;
using std::vector;

class VORPALModule : public Reference {
  OBJ_TYPE(VORPALModule, Reference);
 public:
  bool start(const String &path) {
    return engine_.start(vector<string>(1, path.ascii().get_data())).ok();
  }
  void finish() {
    engine_.finish();
  }
  bool ok() const { return engine_.started(); }
  int eventInstance(const String &name) {
    shared_ptr<vorpal::SoundtrackEvent> event;
    vorpal::Status status = engine_.eventInstance(name.ascii().get_data(), &event);
    if (status.ok()) {
      events_.push_back(event);
      return events_.size()-1;
    }
    std::cout << "[VORPAL-wrap] Failed to instanciate event '"
              << name.ascii().get_data() << "'" << std::endl;
    std::cout << "[VORPAL-wrap] " << status.description() << std::endl;
    return -1;
  }
  void freeEvent(int id) {
    events_[id].reset();
  }
  void clear() {
    events_.clear();
  }
  void pushCommand (int id, const String &cmd) {
    events_[id]->pushCommand(cmd.ascii().get_data());
  }
  void pushCommand1f (int id, const String &cmd, float arg) {
    events_[id]->pushCommand(cmd.ascii().get_data(), arg);
  }
  void setEventPosition (int id, float x, float y, float z) {
    events_[id]->setAudioSource(x, y, z);
  }
  void tick (double dt) {
    engine_.tick(dt);
  }
 protected:
  static void _bind_methods () {
    ObjectTypeDB::bind_method("ok", &VORPALModule::ok);
    ObjectTypeDB::bind_method("start", &VORPALModule::start);
    ObjectTypeDB::bind_method("finish", &VORPALModule::finish);
    ObjectTypeDB::bind_method("event_instance", &VORPALModule::eventInstance);
    ObjectTypeDB::bind_method("free_event", &VORPALModule::freeEvent);
    ObjectTypeDB::bind_method("clear", &VORPALModule::clear);
    ObjectTypeDB::bind_method("set_event_position",
                              &VORPALModule::setEventPosition);
    ObjectTypeDB::bind_method("push_command", &VORPALModule::pushCommand);
    ObjectTypeDB::bind_method("push_command_1f", &VORPALModule::pushCommand1f);
    ObjectTypeDB::bind_method("tick", &VORPALModule::tick);
  }
 private:
  vorpal::Engine                               engine_;
  vector<shared_ptr<vorpal::SoundtrackEvent>>  events_;
};

} // namespace godot
} // namespace vorpal

void initialize_vorpal_module () {
  ObjectTypeDB::register_type<vorpal::godot::VORPALModule>();
}

void uninitialize_vorpal_module () {
  // nothing at all?
}
