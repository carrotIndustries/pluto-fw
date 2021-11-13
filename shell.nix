let
  pkgs = import <nixpkgs> {};
  crossPkgs = import <nixpkgs> {
    # crossSystem = (import <nixpkgs/lib>).systems.examples.msp430;
  crossSystem = {
    config = "msp430-elf";
    libc = "newlib";
    gcc.float = " --enable-multilib --disable-libquadmath ";
  };
  };
  rtttl = ps: ps.callPackage ./rtttl.nix {};
  python = pkgs.python39.withPackages(ps: with ps; [
    (rtttl ps)
    pygobject3
    pyzmq
  ]);
  vscodium = pkgs.vscode-with-extensions.override {
    vscode = pkgs.vscodium;
    vscodeExtensions = [
      pkgs.vscode-extensions.bbenoist.Nix
    ] ++ pkgs.vscode-utils.extensionsFromVscodeMarketplace [{
      name = "beyond-debug";
      publisher = "coolchyni";
      version = "0.9.6";
      sha256 = "1g5qcblnz67786aaqzszrxx6qc80bylyb87aqlgk6423zv4aksq4";
    } {
      name = "vscode-clangd";
      publisher = "llvm-vs-code-extensions";
      version = "0.1.13";
      sha256 = "0qjllvg2wwy3w8gg7q9dkbs0mccxxdq8cg59pbvw0vd8rxn71jpw";
    }];
  };
in
pkgs.mkShell {
    LD_LIBRARY_PATH = "${pkgs.mspds}/lib";
    nativeBuildInputs = with pkgs; [ crossPkgs.buildPackages.gcc crossPkgs.buildPackages.gdb mspds mspdebug zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 gdb vscodium clang ];
}