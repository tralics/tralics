# -- Project information -----------------------------------------------------

project = 'Tralics'
author = 'INRIA and CNRS'
copyright = '2002-2020, INRIA and CNRS'
release = '2.16.0pre'

# -- General configuration ---------------------------------------------------

extensions = ['sphinx_rtd_theme', 'breathe',
              'sphinxcontrib.bibtex', 'sphinxcontrib.katex']
templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
bibtex_bibfiles = ['tralics.bib']

# -- Breathe configuration

breathe_projects = {"tralics": "../xml/"}
breathe_default_project = "tralics"
breathe_default_members = ('members', 'undoc-members')

# -- Options for HTML output -------------------------------------------------

html_show_sourcelink = False
html_static_path = ['_static']
html_theme = 'sphinx_rtd_theme'
html_theme_options = {'prev_next_buttons_location': None}

katex_options = r'''{
    delimiters: [
        { left: "\\(", right: "\\)", display: false },
        { left: "\\[", right: "\\]", display: true },
        { left: "$$", right: "$$", display: true },
        { left: "$", right: "$", display: false }
    ] }'''


def setup(app):
    app.add_css_file('tweak.css')
