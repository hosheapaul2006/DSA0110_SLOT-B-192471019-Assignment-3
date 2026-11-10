# Logger prediction before its first execution

```text
+A +B -B copyAc use -Ac +P -P end -A 
```

There is exactly one trailing space after -A and NO newline in the actual
predicted byte stream. outputs/predicted_output.txt stores those exact bytes.

a constructs A; nested b constructs B and dies at the nested block end. use(a)
copy-constructs an independent Ac parameter, prints use, then destroys that
parameter. The heap P is created and explicitly deleted. main prints end and
then a is destroyed. Passing the named lvalue a requires this copy; C++17 prvalue
elision does not remove it. Record/commit this prediction before compiling or
executing the supplied Logger program, and never rewrite it afterward.
