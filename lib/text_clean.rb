require "text_clean/text_clean"
require "text_clean/version"

module TextClean
  def clean(text, line_sep = "\n")
    # Call the C function
    text_clean(text, line_sep)
  end
end