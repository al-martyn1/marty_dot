/*! \file
    \brief Helpers for Graphviz/dot
 */

#include <string>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace marty {
namespace dot {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Экранирует символ кавычки, символ бэкслеша, а также экранирует пробелы, которые при повторении Graphviz съедает
inline
std::string ecapeLabelString(const std::string &text)
{
    std::string res; res.reserve(text.size());

    std::size_t spaceCnt = 1; // На старте сразу экранируем

    for(auto ch: text)
    {
        switch(ch)
        {
            case '\\':
            {
                res.append(1, '\\');
                res.append(1, ch);
                break;
            }

            case '\"':
            {
                res.append(1, '\\');
                res.append(1, ch);
                break;
            }

            case ' ':
            {
                if (spaceCnt)
                {
                    res.append(1, '\\');
                    res.append(1, ch);
                }
                else
                {
                    res.append(1, ch);
                }
    
                ++spaceCnt;
                break;
            }

            default:
            {
                spaceCnt = 0;
                res.append(1, ch);
            }
        }
    }

    return res;
}

//----------------------------------------------------------------------------
// Пробелы экранировать не нужно
inline
std::string ecapeHtmlLabelString(const std::string &text)
{
    std::string res; res.reserve(text.size());

    for(auto ch: text)
    {
        switch(ch)
        {
            case '&' : res.append("&amp;" ); break;
            case '\"': res.append("&quot;"); break;
            case '\'': res.append("&apos;"); break;
            case '<' : res.append("&lt;"  ); break;
            case '>' : res.append("&gt;"  ); break;
            case '\n': res.append("<br/>" ); break;
            default  : res.append(1, ch);
        }
    }

    return res;
}


//----------------------------------------------------------------------------
} // namespace dot
} // namespace marty

