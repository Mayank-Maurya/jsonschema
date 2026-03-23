#!/bin/sh

set -o errexit
set -o nounset

TMP="$(mktemp -d)"
clean() { rm -rf "$TMP"; }
trap clean EXIT

cat << 'EOF' > "$TMP/source.json"
{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "type": "object",
  "properties": {
    "email": {
      "type": "string"
    },
    "age": {
      "type": "integer"
    },
    "username": {
      "type": "string",
      "maxLength": 100
    }
  }
}
EOF

cat << 'EOF' > "$TMP/target_fail.json"
{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "type": "object",
  "properties": {
    "email": {
      "type": "string",
      "format": "email"
    },
    "age": {
      "type": "integer"
    },
    "username": {
      "type": "string",
      "maxLength": 50
    }
  }
}
EOF

"$1" compatibility "$TMP/source.json" "$TMP/target_fail.json" > /dev/null 2>&1 && EXIT_CODE="$?" || EXIT_CODE="$?"
test "$EXIT_CODE" = "2"
