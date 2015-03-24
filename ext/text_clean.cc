#include <ruby.h>
#include <ruby/encoding.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static rb_encoding* u8_enc;
static rb_encoding* bin_enc;

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
  uint8_t just_added_space = true,   // prevent prefix spaces
          just_added_period = false;
  for (read = text; read < eos; read++) {
    char c = *read;
    if (c >= 'A' && c <= 'Z') {
      // Change upper case to lowercase
      c += 32;
    } else if (c == '\t' || c == ',' || c == '&' || c == '/') {
      // Change inconsequential punctuation to spaces (i.e. all count as whitespace)
      c = ' ';
    } else if (c == '?' || c == '!' || c == ':' || c == ';') {
      // Change exclamation, question marks to periods (i.e. sentence boundaries)
      c = '.';
    }

    if (c == '-') {
      // double dash?
      if (*(read + 1) == '-') {
        *write++ = ' ';
        read++;
      } else {
        // scan ahead to see if this is a hyphen at the end of the line
        char* scan_ahead;
        for (scan_ahead = read + 1; scan_ahead < eos; scan_ahead++) {
          char s = *scan_ahead;
          if (s != '\t' && s != ' ') {
            if (s == '\n') {
              // this is a hyphenated line join, so join the lines
              read = scan_ahead;
              break;
            } else {
              // not a line join
              break;
            }
          }
        }
      }
    } else if (c == '.' && !just_added_period) {
      // erase space before period
      if (just_added_space) write--;
      *write++ = line_sep;
      just_added_period = true;
      just_added_space = false;
    } else if (c == ' ' && !just_added_space && !just_added_period) {
      *write++ = ' ';
      just_added_space = true;
      just_added_period = false;
    } else if (c >= 'a' && c <= 'z') {
      *write++ = c;
      just_added_space = false;
      just_added_period = false;
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
void Init_text_clean() {
    VALUE rb_mText = rb_define_module("TextClean");

    u8_enc = rb_utf8_encoding();
    bin_enc = rb_ascii8bit_encoding();

    rb_define_module_function(rb_mText, "text_clean", RUBY_METHOD_FUNC(text_clean), 2);
}
