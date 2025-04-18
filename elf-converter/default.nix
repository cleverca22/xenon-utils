{ stdenv }:

stdenv.mkDerivation {
  name = "elf-converter";
  src = ./.;
  installPhase = ''
    mkdir -pv $out/bin
    cp elf-converter $out/bin/
  '';
}
