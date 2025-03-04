{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, utils, nixpkgs }:
  (utils.lib.eachDefaultSystem (system:
  let
    pkgs = nixpkgs.legacyPackages.${system}; # .extend self.overlay;
  in {
    packages.nand-explorer = pkgs.callPackage ./nand-explorer.nix {};
  }));
}
