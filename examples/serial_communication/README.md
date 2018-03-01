# Serial communication

## Display capabilities
 * 2 lines (32x8 pixels) of 6 ASCII characters (6x4 pixels);
 * 3 colors : red, green, orange;
 * 1 set of icons (8x8 pixels);
 * scolling of one or both text lines, in left direction or right direction;

## Principle

To display a text, simply send it to the serial port.

To change the way the color or position of the text  is displayed, a set of commands is available.

A command is prefixed with the character `/` and can be followed by one argument. For instance, to change the cursor color before sending _Hello_ :

```
> /color green\n
> hello\n
```

Commands and texts can be send inline. For instance, the previous example can be send this way :
```
> /color green hello\n
```

## List of commands

<table>
  <tr>
    <th>Syntax</th><th>Command</th><th>Arguments</th><th>Default value</th>
  </tr>
  <tr><td><code>/clear</code></td><td>clear screen</td><td>-</td><td>-</td></tr>
  <tr><td><code>/align</code></td><td>cursor horizontal position</td><td><code>left</code>; <code>center</code>; <code>right</code></td><td><code>left</code></td></tr>
  <tr><td><code>/line</code></td><td>cursor vertical position</td><td><code>1</code>; <code>2</code></td><td><code>1</code></td></tr>
  <tr><td><code>/color</code></td><td>cursor color</td><td><code>red</code>; <code>green</code>; <code>orange</code>; <code>-red</code>; <code>-green</code>; <code>-orange</code></td><td><code>red</code></td></tr>
  <tr><td><code>/int</code> ; <code>/intensity</code></td><td>brightness</td><td>between <code>0</code> and <code>15</code></td><td><code>0</code></td></tr>
  <tr><td><code>/scroll</code></td><td>scrolling</td><td><code>left</code>; <code>right</code>; <code>stop</code></td><td><code>stop</code></td></tr>
  <tr><td><code>/fixed</code></td><td>disable scrolling or blinking on half the display</td><td><code>none</code>; <code>1</code>; <code>2</code>; <code>both</code></td><td><code>none</code></td></tr>
  <tr><td><code>/blink</code></td><td>make it blink !</td><td><code>on</code>; <code>off</code></td><td><code>off</code></td></tr>
  <tr><td><code>/icon</code></td><td>displays icon</td><td><code>1</code>; <code>2</code>; ... <code>n</code>;</td><td>-</td></tr>
  <tr><td><code>/flow</code></td><td>show a scrolling or blinking arrow</td><td><code>left</code>; <code>right</code>; <code>up</code>; <code>down</code></td><td>-</td></tr>
</table>
