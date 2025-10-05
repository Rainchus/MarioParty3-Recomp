{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    nixpkgs-old-clang.url = "github:NixOS/nixpkgs/nixos-24.11"; # may be not be required
    flake-utils.url = "github:numtide/flake-utils"; # TODO dont use flake-utils
    # TODO: take N64Recomp and RSPRecomp as inputs
  };
  outputs = { self, nixpkgs, nixpkgs-old-clang, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        pkgs-old-clang = import nixpkgs-old-clang { inherit system; };
      in {
        # also nix-ld is required at a NixOS level because rt64 vendors binaries(?)
        # TODO: fix this in rt64
        devShells.default = pkgs.mkShell {
          name = "marioparty3-recomp";
          packages = with pkgs; [
            cmake
            ninja
            pkgs-old-clang.clang
            pkg-config
            sdl2-compat
            gtk3
            wrapGAppsHook3
          ];
        };
        # cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
      }
    );
}
