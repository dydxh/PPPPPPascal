#ifndef __ASTVIS__H__
#define __ASTVIS__H__

#include "context.hpp"
#include "AST/frame.hpp"

namespace yapc {
    class ASTvis {
    public:
        ASTvis() = default;
        ~ASTvis() = default;
        void travAST(Context& context);

    private:
        std::string filename  = "yapc.output.tex";
        std::string texHeader = "\\documentclass{minimal} \n\
\\usepackage[a4paper,margin=1cm,landscape]{geometry} \n\
\\usepackage{tikz} \n\n\
\%\%\%< \n\
\\usepackage{verbatim} \n\
\\usepackage[active,tightpage]{preview} \n\
\\PreviewEnvironment{tikzpicture} \n\
\\setlength\\PreviewBorder{5pt}% \n\%\%\%> \n\n\
\\begin{comment} \n\
:Title:  Rule based diagram \n\n\
\\end{comment} \n\n\
\\usetikzlibrary{positioning,shadows,arrows} \n\n\
\\begin{document}\n\
\\begin{center}\n\
\\begin{tikzpicture}[\n\
    fact/.style={rectangle, draw=none, rounded corners=1mm, fill=blue, drop shadow,\n\
        text centered, anchor=north, text=white},\n\
    state/.style={circle, draw=none, fill=orange, circular drop shadow,\n\
        text centered, anchor=north, text=white},\n\
    leaf/.style={circle, draw=none, fill=red, circular drop shadow,\n\
        text centered, anchor=north, text=white},\n\
    level distance=0.5cm, growth parent anchor=south\n\
]\n";
        std::string texTail = ";\n\n\\end{tikzpicture}\n\\end{center}\n\\end{document}\n";
    };
}

#endif