#pragma once

/*
 * Template to easily check if size of message equals size of target, example use:
 * assert_size<sizeof(message.settings), SetPointSimple::sizeof_Settings> check;
 * this is a template to easily print the size in the error message.
 *
 * In SetPointSimple, struct Settings is usually private, so it also implements the public static const int sizeof_Settings
 */
template<int s, int t>
void assert_size() {
  static_assert(s == t, "size mismatch");
};

