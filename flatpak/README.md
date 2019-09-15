squiddio flatpak README
-----------------------

This is a simple packaging to be use the squiddio plugin when using the opencpn's
flatpak package. To build and install:

  - Install flatpak and flatpak-builder as described in https://flatpak.org/
  - Install the opencpn and opencpn base plugin flatpak packages. Using  the
    provisionary test repo at fedorapeople.org do:

      $ flatpak install --user \
          https://leamas.fedorapeople.org/opencpn/opencpn.flatpakref
      $ flatpak install --user \
          https://leamas.fedorapeople.org/opencpn/opencpn-plugin-base.flatpakref

  - The squiddio plugin can now be built and installed using

      $ make
      $ make install

The actual version built depends on the tag: stanza in the yaml file; update to
other versions as preferred.
