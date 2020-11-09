#include "tralics/ColSpec.h"
#include "tralics/Xml.h"

// This creates a new color item, to be pushed on the color stack
// Note that used is false, set to true by get_id.
ColSpec::ColSpec(std::string a, std::string b, std::string c) : name(std::move(a)), model(std::move(b)), value(std::move(c)), used(false) {
    xval = new Xml(the_names["color"], nullptr);
    if (!name.empty()) xval->id.add_attribute(std::string("name"), name);
    xval->id.add_attribute(std::string("model"), std::string(model));
    xval->id.add_attribute(std::string("value"), std::string(value));
    static int n = 0;
    id           = std::string(fmt::format("colid{}", ++n)); // This is a unique id
    xval->id.add_attribute(the_names["id"], id);
}
