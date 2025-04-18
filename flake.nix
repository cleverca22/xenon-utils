{
  inputs = {
    utils.url = "github:numtide/flake-utils";
    nixpkgs-ppc.url = "github:cleverca22/nixpkgs/ugly-test";
  };
  outputs = { self, utils, nixpkgs, nixpkgs-ppc }:
  (utils.lib.eachSystem [ "x86_64-linux" "ppc64" "ppc32" ] (system:
  let
    pkgsLut = {
      ppc32 = import nixpkgs-ppc {
        crossSystem = {
          config = "powerpc-none-eabi";
          libc = "newlib";
        };
        system = "x86_64-linux";
      };
      ppc64 = import nixpkgs-ppc {
        crossSystem = {
          config = "powerpc64-unknown-linux-gnuabielfv2";
        };
        system = "x86_64-linux";
      };
      x86_64-linux = nixpkgs.legacyPackages.${system};
    };
    pkgs = pkgsLut.${system};
  in {
    packages = {
      nand-explorer = pkgs.callPackage ./nand-explorer.nix {};
      baremetal-test = pkgs.callPackage ./baremetal-test {};
      elf-converter = pkgs.callPackage ./elf-converter {};
    };
  }));
}
