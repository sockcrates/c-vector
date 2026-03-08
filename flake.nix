{
  description = "c-vector development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f system);
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "c-vector";
            version = "0.1.0";
            src = ./.;

            nativeBuildInputs = with pkgs; [
              llvmPackages_18.clang
              gnumake
            ];

            buildPhase = ''
              export CC=clang
              make build_lib
            '';

            installPhase = ''
              mkdir -p $out/lib $out/include
              cp build/libvector.a $out/lib/
              cp -r include/. $out/include/
            '';
          };
        });

      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              llvmPackages_18.clang
              gnumake
            ];

            shellHook = ''
              export CC=clang
            '';
          };
        });
    };
}
