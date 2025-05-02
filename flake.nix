{
  description = "Development environment for ACS Undergrad";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs =
    {
      nixpkgs,
      ...
    }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages."${system}";
    in
    {
      devShell."${system}" = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
        packages = with pkgs; [
          # WARNING: Do NOT add gcc, clang, gligc, libcxx, etc.
          # They will break the environment, leading to compilation or LSP errors.
          clang-tools
          meson
          ninja
          just
          fd
        ];
      };
    };
}
