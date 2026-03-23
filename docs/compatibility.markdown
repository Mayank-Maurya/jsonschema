Compatibility
=============

```sh
jsonschema compatibility <source.json> <target.json>
  [--mode/-m <backward|forward|full>] [--output/-o <text|json>]
  [--strict] [--quiet/-q]
```

When you update a JSON schema, you want to be sure that your changes won't break your existing applications. The `compatibility` command takes two schemas—an old one and a new one—and identifies if the new one introduces backwards, forwards, or full breaking changes.

Examples
--------

### Determine compatibility of backward changes

```sh
jsonschema compatibility old_schema.json new_schema.json --mode backward
```

### Use strict mode where all warnings are treated as breaking errors

```sh
jsonschema compatibility old_schema.json new_schema.json --strict
```

### Output machine-readable JSON object to pipe to other tools

```sh
jsonschema compatibility old_schema.json new_schema.json --output json
```

### Suppress text output natively

```sh
jsonschema compatibility old_schema.json new_schema.json --quiet
```
