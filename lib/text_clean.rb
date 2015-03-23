require "text_clean_ext"
require "text_clean/version"

module Text
  def clean(text, line_sep = "\n")
    # Call the C function
    text_clean(text, line_sep)
  end
end