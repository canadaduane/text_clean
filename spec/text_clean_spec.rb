require 'rspec'
require 'text_clean'

def clean(text, line_sep="\n", &block)
  TextClean.clean(text, line_sep)
  block.call(text)
end

def clean_eq(text, expected_after_cleaning, line_sep="\n")
  clean(text, line_sep) { |t| expect(t).to eq(expected_after_cleaning) }
end

describe TextClean do
  it "lowercases text" do
    clean_eq("ALL UPPERCASE", "all uppercase")
  end

  it "compacts whitespace" do
    clean_eq("is\t it    so?", "is it so\n")
  end

  it "treats [,/&] as whitespace" do
    clean_eq("a,bb&cc/d", "a bb cc d")
  end

  it "treats [;:!?] as sentence separators" do
    clean_eq("x;y?z!:q", "x.y.z.q", ".")
  end

  it "joins hyphenated words at line end" do
    clean_eq("satis-\nfaction", "satisfaction")
  end

  it "ignores whitespace after hyphen at line end" do
    clean_eq("satis- \t \nfaction", "satisfaction")
  end

  it "treats a double hyphen as word separator" do
    clean_eq("good--do it", "good do it")
  end

  it "compacts whitespace around double hyphen" do
    clean_eq("good -- do it", "good do it")
  end

  it "treats newlines as word separator" do
    clean_eq("a\nb\nc", "a b c")
  end

  it "ignores numbers" do
    clean_eq("123abc", "abc")
  end

  it "ignores quotes and braces" do
    clean_eq("[jim] \"speaks\"", "jim speaks")
  end

  it "keeps apostrophes" do
    clean_eq("dad's", "dad's")
  end
end