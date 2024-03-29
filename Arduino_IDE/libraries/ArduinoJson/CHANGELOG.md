ArduinoJson: change log
=======================

v5.0 (currently under development)
----

* Added support of `String` class (issue #55, #56, #70, #77)
* Redesigned `JsonVariant` to leverage converting constructors instead of assignment operators
* Switched to new library layout (requires Arduino 1.0.6 or above)

**BREAKING CHANGES**:
- `JsonObject::add()` was renamed to `set()`
- `JsonArray::at()` and `JsonObject::at()` were renamed to `get()`
- Number of digits of floating point value are now set with `double_with_n_digits()`

**Personal note about the `String` class**:
Support of the `String` class has been added to the library because many people use it in their programs.
However, you should not see this as an invitation to use the `String` class.
The `String` class is **bad** because it uses dynamic memory allocation.
Compared to static allocation, it compiles to a bigger, slower program, and is less predicate.
You certainly don't want that in an embedded environment!

v4.4
----

* Added `JsonArray::measureLength()` and `JsonObject::measureLength()` (issue #75)

v4.3
----

* Added `JsonArray::removeAt()` to remove an element of an array (issue #58)
* Fixed stack-overflow in `DynamicJsonBuffer` when parsing huge JSON files (issue #65)
* Fixed wrong return value of `parseArray()` and `parseObject()` when allocation fails (issue #68)

v4.2
----

* Switched back to old library layout (issues #39, #43 and #45)
* Removed global new operator overload (issue #40, #45 and #46)
* Added an example with EthernetServer

v4.1
----

* Added DynamicJsonBuffer (issue #19)

v4.0
----

* Unified parser and generator API (issue #23)
* Updated library layout, now requires Arduino 1.0.6 or newer

**BREAKING CHANGE**: API changed significantly, see [Migrating code to the new API](https://github.com/bblanchon/ArduinoJson/wiki/Migrating-code-to-the-new-API).


v3.4
----

* Fixed escaped char parsing (issue #16)


v3.3
----

* Added indented output for the JSON generator (issue #11), see example bellow.
* Added `IndentedPrint`, a decorator for `Print` to allow indented output

Example:

    JsonOject<2> json;
    json["key"] = "value";
    json.prettyPrintTo(Serial);

v3.2
----

* Fixed a bug when adding nested object in `JsonArray` (bug introduced in v3.1).

v3.1
----

* Calling `Generator::JsonObject::add()` twice with the same `key` now replaces the `value`
* Added `Generator::JsonObject::operator[]`, see bellow the new API
* Added `Generator::JsonObject::remove()` (issue #9)

Old generator API:

	JsonObject<3> root; 
    root.add("sensor", "gps");
    root.add("time", 1351824120);
    root.add("data", array);

New generator API:

	JsonObject<3> root; 
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    root["data"] = array;

v3.0
----

* New parser API, see bellow
* Renamed `JsonHashTable` into `JsonObject`
* Added iterators for `JsonArray` and `JsonObject` (issue #4)

Old parser API:

    JsonHashTable root = parser.parseHashTable(json);

	char*  sensor    = root.getString("sensor");
	long   time      = root.getLong("time");
	double latitude  = root.getArray("data").getDouble(0);
    double longitude = root.getArray("data").getDouble(1);

New parser API:

	JsonObject root = parser.parse(json);

	char*  sensor    = root["sensor"];
    long   time      = root["time"];
    double latitude  = root["data"][0];
    double longitude = root["data"][1];

v2.1
----

* Fixed case `#include "jsmn.cpp"` which caused an error in Linux (issue #6)
* Fixed a buffer overrun in JSON Parser (issue #5)

v2.0
----

* Added JSON encoding (issue #2)
* Renamed the library `ArduinoJsonParser` becomes `ArduinoJson`

**Breaking change**: you need to add the following line at the top of your program.

	using namespace ArduinoJson::Parser;

v1.2
----

* Fixed error in JSON parser example (issue #1)

v1.1
----

* Example: changed `char* json` into `char[] json` so that the bytes are not write protected
* Fixed parsing bug when the JSON contains multi-dimensional arrays

v1.0 
----

Initial release
