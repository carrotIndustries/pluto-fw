let
  pinnedPkgs = import (builtins.fetchTarball {
    name = "nixos-unstable-2021-11-15";
    url = "https://github.com/nixos/nixpkgs/archive/2fa862644fc15ecb525eb8cd0a60276f1c340c7c.tar.gz";
    sha256 = "00l884zydbrww2jxjvf62sm1y96jvys22jg9vb3fsznz2mbz41jb";
  });
  
  pkgs = pinnedPkgs {};

  crossPkgs = pinnedPkgs {
    crossSystem = {
      config = "msp430-elf";
      libc = "newlib";
      gcc.float = " --enable-multilib --disable-libquadmath ";
    };
  };

  python = pkgs.python39.withPackages(ps: with ps; [
    (buildPythonPackage rec {
       pname = "rtttl";
       version = "0.2";
       src = fetchPypi {
         inherit pname version;
         sha256 = "02h23mp9mvd9d7hsvj280k3drms9b1dbla4kar6m2fjdnhd9i9jq";
      };
    })
    pygobject3
    pyzmq
  ]);
  vscode = pkgs.vscode-with-extensions.override {
    vscode = pkgs.vscode;
    vscodeExtensions = [
      pkgs.vscode-extensions.bbenoist.nix
      pkgs.vscode-extensions.ms-vscode.cpptools
    ] ++ pkgs.vscode-utils.extensionsFromVscodeMarketplace [{
      name = "msp430-assembly";
      publisher = "zacharypenn";
      version = "1.0.0";
      sha256 = "1pjx57bsjkkly0110drhgxycx17m2hvgn5ppcawwsg0gg86k9v2a";
    }
    {
      name = "linkerscript";
      publisher = "zixuanwang";
      version = "1.0.2";
      sha256 = "0rr5mz8g8myskgixiw76rwda8g955a1al8kk4s30b0byfaszia17";
    }
    {
      name = "gnu-mapfiles";
      publisher = "trond-snekvik";
      version = "1.1.0";
      sha256 = "0416qprq9rnzl9na3fxhs9wnmws9zfm473qxnvji2vy752l4wxr4";
    }
    ];
  };
in
pkgs.mkShell {
    LD_LIBRARY_PATH = "${pkgs.mspds}/lib";
    nativeBuildInputs = with pkgs; [ crossPkgs.buildPackages.gcc crossPkgs.buildPackages.gdb mspds mspdebug zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 lsof gdb vscode];
}