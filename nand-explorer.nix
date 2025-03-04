{ stdenv }:

stdenv.mkDerivation {
  name = "nand-explorer";
  src = ./nand-explorer;
  allowSubstitutes = false;
}
