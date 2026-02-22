class Tralics < Formula
  desc "LaTeX to XML translator"
  homepage "https://github.com/tralics/tralics"
  license "CECILL-2.1"
  head "https://github.com/tralics/tralics.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "ninja" => :build

  def install
    system "cmake", "-S", ".", "-B", "build", "-G", "Ninja", *std_cmake_args
    # Build only the executable target: the default target runs the full test suite.
    system "cmake", "--build", "build", "--target", "tralics"
    system "cmake", "--install", "build"
  end

  test do
    (testpath/"hello.tex").write <<~EOS
      \\documentclass{article}
      \\begin{document}
      Hello from Homebrew.
      \\end{document}
    EOS

    system bin/"tralics", "hello.tex"
    assert_predicate testpath/"hello.xml", :exist?
  end
end
