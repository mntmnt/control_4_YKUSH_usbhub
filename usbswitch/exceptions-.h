#pragma once

#include <stdexcept>
#include <QString>

namespace usbswitch::details {

struct FailToOpenException : public std::runtime_error {
    explicit FailToOpenException(const QString & error):
        runtime_error("Failed to open device"),
        error { error } {
    }


    const QString & errorMessage() const noexcept {
        return error;
    }

private:

    QString error;
};

}
