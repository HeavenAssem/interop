//
// Created by islam on 06.05.17.
//

#include "exceptions.h"

using namespace std;


namespace mosaic {
    error::error(const string_view &what): message(what.data()) {

    }

    void error::append(const string_view &message) {
        this->message += message.data();
    }

    const char * error::what() const noexcept {
        return message.c_str();
    }

    not_implemented::not_implemented(const std::string_view & what) : error(what) {

    }

    open_error::open_error(const std::string_view & what) : error(what) {

    }

    validation_error::validation_error(const std::string_view & what) : error(what) {

    }

    module_validation_error::module_validation_error(const std::string_view & what) : validation_error(what) {

    }

    open_path_error::open_path_error(const std::string_view & what) : open_error(what) {

    }

    open_file_error::open_file_error(const std::string_view & what) : open_error(what) {

    }

    loading_error::loading_error(const string_view &what): error(what) {

    }

    node_loading_error::node_loading_error(const std::string_view & what) : loading_error(what) {

    }

    library_loading_error::library_loading_error(const string_view &what): loading_error(what) {

    }

    symbol_loading_error::symbol_loading_error(const string_view &what): loading_error(what) {

    }

    library_unloading_error::library_unloading_error(const string_view &what): loading_error(what) {

    }

    lookup_error::lookup_error(const std::string_view & what) : error(what) {

    }

    module_lookup_error::module_lookup_error(const std::string_view & what) : lookup_error(what) {

    }

    function_lookup_error::function_lookup_error(const std::string_view & what) : lookup_error(what) {

    }

    function_call_error::function_call_error(const std::string_view & what) : error(what) {

    }
}
