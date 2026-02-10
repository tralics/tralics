#include "tralics/ColSpec.h"
#include "tralics/Xml.h"

// This creates a new color item, to be pushed on the color stack
// Note that used is false, set to true by get_id.
ColSpec::ColSpec(std::string a, std::string b, std::string c)
    : name(std::move(a)), model(std::move(b)), value(std::move(c)), xval(new Xml(the_names["color"], nullptr)) {
    if (!name.empty()) xval->add_att("name", name);
    xval->add_att("model", model);
    xval->add_att("value", value);
    static int n = 0;
    id           = fmt::format("colid{}", ++n); // This is a unique id
    xval->add_att(the_names["id"], id);
}
