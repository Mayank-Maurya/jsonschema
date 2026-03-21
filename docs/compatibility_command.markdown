# JSON Schema Compatibility Checker

When you update a JSON schema, you want to be sure that your changes won't break your existing apps. That's exactly what the `compatibility` command does! It takes two schemas—an old one and a new one—and tells you if the new one has any breaking changes.

## How to run it

To check if your new schema is safe to use, just run this command in your terminal:

```bash
jsonschema compatibility old_schema.json new_schema.json
```

The tool expects your files to be in valid JSON format.

## Command Options

To make the command fit exactly what you need, you can add a few extra flags:

* `-m` or `--mode`: Tells the checker how strict it should be. 
  * `backward` (Default): Checks if your old apps can safely read data made with the new schema. 
  * `forward`: Checks if your new apps can safely read data made with the old schema.
  * `full`: Checks both ways at once. This is the strictest mode!
* `-o` or `--output`: Changes how the results look.
  * `text` (Default): Shows a nice, colored, easy-to-read summary right in your terminal.
  * `json`: Outputs raw JSON data. This is perfect if you want to use the command inside an automated system or pipeline (like GitHub Actions).
* `--strict`: Sometimes changing a description or a small detail is just a "warning". This option treats every single warning as a hard breaking change.
* `-q` or `--quiet`: Hides all the normal text output. It will only print something if there is a real error (like a missing file).
* `--no-color`: Turns off the pretty colors in the terminal. (This happens automatically if you run the command through a script).

## Understanding the Results

### 1. If everything is good
If there are no breaking changes, the command will show a green checkmark and say you are good to go. Behind the scenes, the process finishes with an **Exit Code of 0**.

**Example Input & Output:**
```text
$ jsonschema compatibility v1.json v2_compatible.json

✔ Compatibility check passed (mode: backward).
  No breaking changes detected between v1.json and v2_compatible.json.
```


### 2. If things break
If you made a change that breaks the rules (like requiring a brand new field that old apps don't know about), the command will list exactly what broke and where in the file it happened. It highlights warnings in yellow and breaking changes in red.

When this happens, the process finishes with an **Exit Code of 2**.

**Example Input & Output (Text Mode):**
```text
$ jsonschema compatibility v1.json v3_breaking.json

✖ Compatibility check failed (mode: backward).
  Found 2 breaking changes and 1 warning:

  [BREAKING] /properties/email
    - Constraint tightened: added "format": "email".

  [BREAKING] /properties/age
    - Type narrowed: removed "string" from type array.

  [WARNING]  /properties/username
    - "maxLength" reduced from 100 to 50.
```

**Example Input & Output (JSON Mode):**
If you pass the `--output json` flag (or if the command is being piped to another program automatically), you get raw data you can easily process using tools like `jq` instead of pretty colors:
```json
$ jsonschema compatibility v1.json v3_breaking.json --output json

{
  "mode": "backward",
  "compatible": false,
  "breaking_changes": [
    {
      "path": "/properties/email",
      "message": "Constraint tightened: added 'format': 'email'."
    },
    {
      "path": "/properties/age",
      "message": "Type narrowed: removed 'string' from type array."
    }
  ],
  "warnings": [
    {
      "path": "/properties/username",
      "message": "'maxLength' reduced from 100 to 50."
    }
  ]
}
```

### 3. If there is an error
If you forget to include a file, or if the file isn't actually a proper JSON document, the command will give you a standard error message and finish with an **Exit Code of 1**.
```text
$ jsonschema compatibility only_one_file.json

Error: This command expects a path to a source schema and a target schema
Usage: jsonschema compatibility path/to/source.json path/to/target.json
```
