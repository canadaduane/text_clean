#include <ruby.h>
#include <ruby/encoding.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/** Transforms text such as the following:
 *
 *   And behold, I said, "This is no good!"
 *   What shall ye say unto these people, there-
 *   fore?
 *
 * Into a cleaned up single line of text, like the following:
 *
 *   and behold i said this is no good.what shall ye say unto these people therefore.
 *
 * Spaces indicate word boundaries, while periods indicate sentence boundaries.
 */
size_t text_clean_cstr(char* text, long len, char line_sep)
{
  if (len <= 0) return 0;

  char* eos = text + (size_t)len;
  char* read;
  char* write = text;
  uint8_t join_lines = false,
          just_added_space = true,   // prevent prefix spaces
          just_added_period = false;
  for (read = text; read < eos; read++) {
    char c = *read;
    if (c >= 'A' && c <= 'Z') {
      // Change upper case to lowercase
      c += 32;
    } else if (c == '\n' || c == '\t' || c == ',' || c == ';' || c == '&' || c == '/') {
      // Change newlines to spaces (i.e. both count as whitespace)
      c = ' ';
    } else if (c == '?' || c == '!' || c == ':') {
      // Change exclamation, question marks to periods (i.e. sentence boundaries)
      c = line_sep;
    }

    if (c == '-') {
      join_lines = true;
    } else if (join_lines && c == ' ') {
      // ignore whitespace after a dash (i.e. including newlines, which is the
      // most common case because words that are broken by syllables are dashed)
    } else if (c == line_sep && !just_added_period) {
      // erase space before period
      if (just_added_space) write--;
      *write++ = line_sep;
      just_added_period = true;
      just_added_space = false;
      join_lines = false;
    } else if (c == ' ' && !just_added_space && !just_added_period) {
      *write++ = ' ';
      just_added_space = true;
      just_added_period = false;
    } else if (c >= 'a' && c <= 'z') {
      *write++ = c;
      just_added_space = false;
      just_added_period = false;
      join_lines = false;
    }
  }
  // erase space at end of text
  if (just_added_space) write--;

  // Return the new length of the string
  return (size_t)(write - text);
}

static VALUE text_clean(VALUE self, VALUE text, VALUE ending) {
  char*   ptext = RSTRING_PTR(text);
  long      len = RSTRING_LEN(text);
  char line_sep = '.';

  if (RSTRING_LEN(ending) == 1) {
    line_sep = RSTRING_PTR(ending)[0];
  }

  rb_str_modify(text);

  size_t new_length = text_clean_cstr(ptext, len, line_sep);

  rb_str_set_len(text, (long)new_length);

  return text;
}

extern "C"
void Init_text_clean_ext() {
    VALUE rb_mText = rb_define_module("TextClean");

    u8_enc = rb_utf8_encoding();
    bin_enc = rb_ascii8bit_encoding();

    rb_define_module_function(rb_mText, "text_clean", RUBY_METHOD_FUNC(text_clean), 2);
}
