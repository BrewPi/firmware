#include "Object.h"
#include "Commands.h"
#include "Integration.h"
#include <memory>

std::shared_ptr<cbox::Object> createApplicationObject(cbox::obj_type_t typeId, cbox::DataIn& in, cbox::CommandError& errorCode);
