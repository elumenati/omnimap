1,$s/^ /0x/
1,$s/ /,0x/g
1,$s/$/,/g
1s/^/static int fs6[] = {/
$s/,$/};/
