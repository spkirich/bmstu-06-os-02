{
  description = "https://github.com/spkirich/bmstu-06-os-02";

  outputs = { self, nixpkgs }:

    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

    in
    {
      apps.${system} = {
        client = {
          program = "${self.packages.${system}.default}/bin/client";
          type = "app";
        };

        server = {
          program = "${self.packages.${system}.default}/bin/server";
          type = "app";
        };
      };

      devShells.${system} = {
        default = pkgs.mkShell {
          buildInputs = with pkgs; [
            self.packages.${system}.default
            man-pages
          ];
        };
      };

      formatter.${system} = pkgs.nixpkgs-fmt;

      packages.${system} = {
        default = pkgs.stdenv.mkDerivation {
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
            mv client server $out/bin
          '';

          src = ./src;
        };
      };
    };
}
