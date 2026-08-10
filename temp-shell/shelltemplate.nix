# shell.nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  # packages you want available in the environment
  packages = with pkgs; [
    cowsay
    lolcat
  ];
}

