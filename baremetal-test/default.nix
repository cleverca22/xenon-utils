{ stdenv }:

stdenv.mkDerivation {
  name = "baremetal-test";

  src = ./.;
}
