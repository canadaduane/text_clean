# replace newlines with periods to make regexes easier
tr '\n' '.'                           |
# convert sentence endings to periods to be consistent
tr ':' '.'                            |

tr ';' '.'                            |

tr '?' '.'                            |

tr '!' '.'                            |
# compact line endings
sed -E $'s/\.+/./g'                   |
# join lines ending in hyphenation
sed -E $'s/- *\.//g'                  |
# replace periods with newlines
sed -E $'s/[ \\\t]*\.[ \\\t]*/\\\n/g' |
# compact whitespace-line characters
sed -E $'s/[&\/, \\\t\-]+/ /g'        |
# remove non-alpha characters
sed -E $'s/[^a-zA-Z\' ]//g'           |
# strip leading whitespace
sed -E $'s/^ *//'                     |
# convert to lowercase
tr '[:upper:]' '[:lower:]'