{
  description = "Операционные системы. Лабораторная работа № 2";

  outputs = { self, nixpkgs }:

    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        name = "bmstu-06-os-02";

        buildInputs = with pkgs; [
          gcc
        ];

        buildPhase = ''
          gcc client.c -o client
          gcc server.c -o server
        '';

        installPhase = ''
          mkdir -p $out/bin

          mv client $out/bin
          mv server $out/bin
        '';

        src = ./.;
      };

      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          self.packages.${system}.default
          man-pages
        ];
      };

      formatter.${system} = pkgs.nixpkgs-fmt;
    };
}
