#ifndef __ASTVIS__H__
#define __ASTVIS__H__

#include "context.hpp"
#include "AST/frame.hpp"

#include <fstream>

namespace yapc {
    class ASTvis {
    public:
        ASTvis() : of("yapc.output.tex", of.trunc | of.out)
        {
            if (!of.is_open())
            {
                std::cout << "failed to open file\n";
            }
        }
        ~ASTvis() = default;
        void travAST(Context& context);

    private:
        int travProgram(const std::shared_ptr<ProgramAST>& program);
        int travProgramBody(const std::shared_ptr<ProgramAST>& program);

        int travCONST(const std::shared_ptr<ConstDeclListAST>& const_declListAST);
        int travTYPE(const std::shared_ptr<TypeDeclListAST>& type_declListAST);
        int travVAR(const std::shared_ptr<VarDeclListAST>& var_declListAST);
        int travSubprocList(const std::shared_ptr<ProgListAST>& subProc_declListAST);
        int travSubproc(const std::shared_ptr<ProgramAST>& subProc_AST);
        int travCompound(const std::shared_ptr<CompoundStmtAST>& compound_declListAST);

        int travStmt(const std::shared_ptr<StmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<IfStmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<WhileStmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<ForStmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<RepeatStmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<CallStmtAST>&p_stmp);
        int travStmt(const std::shared_ptr<AssignStmtAST>&p_stmp);

        int travExpr(const std::shared_ptr<ExprAST>& expr);
        int travExpr(const std::shared_ptr<BinaryExprAST>& expr);
        int travExpr(const std::shared_ptr<UnaryExprAST>& expr);
        int travExpr(const std::shared_ptr<ArrayAccessAST>& expr);
        int travExpr(const std::shared_ptr<RecordAccessAST>& expr);
        int travExpr(const std::shared_ptr<FuncCallAST>& expr);
        int travExpr(const std::shared_ptr<CustomFuncAST>& expr);
        int travExpr(const std::shared_ptr<SysFuncAST>& expr);

        int node_cnt    = 0;
        int subproc_cnt = 0;
        int stmt_cnt    = 0;
        int leaf_cnt    = 0;
        std::fstream of;
        std::string texHeader = "\\documentclass{minimal} \n\
\\usepackage{tikz} \n\n\
\%\%\%< \n\
\\usepackage{verbatim} \n\
\\usepackage[active,tightpage]{preview} \n\
\\PreviewEnvironment{tikzpicture} \n\
\\setlength\\PreviewBorder{5pt}% \n\%\%\%> \n\n\
\\begin{comment} \n\
:Title: AST                 \n\
:Tags: Trees; Syntax        \n\
:Author: Eric Morris        \n\
:Slug: abstract-syntax-tree \n\
\\end{comment} \n\n\n\n\
\\usetikzlibrary{trees}  \n\n\n\
\\begin{document}\n\
\\tikzstyle{every node}=[draw=black,thick,anchor=west]  \n\
\\tikzstyle{selected}=[draw=red,fill=red!30]            \n\
\\tikzstyle{optional}=[dashed,fill=gray!50]             \n\
\\begin{tikzpicture}[\n\
    grow via three points={one child at (0.5,-0.7) and   \n\
    two children at (0.5,-0.7) and (0.5,-1.4)},          \n\
    edge from parent path={(\\tikzparentnode.south) |- (\\tikzchildnode.west)}  \n\
]\n";
        std::string texTail = ";\n\n\\end{tikzpicture}\n\\end{document}\n";
        std::string texNone = "child [missing] {}\n";
    };
}

#endif