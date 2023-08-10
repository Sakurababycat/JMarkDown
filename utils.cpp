#include "utils.h"
#include "QString"
#include "QRegularExpression"

QString md2html( const QString &mdText )
{
    QString				htmlText = mdText;
    static QRegularExpression	rmexbrrex( "<br>?(<h\\d>)|(</h\\d>)<br>?" );
    static QRegularExpression	h1rex( "^# +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	h2rex( "^## +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	h3rex( "^### +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	h4rex( "^#### +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	h5rex( "^##### +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	h6rex( "^###### +(.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	ulrex( "^((\t| {4})*)(\\*|-) (.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	olrex( "^((\t| {4})*)(\\d+\\.) (.+)$", QRegularExpression::MultilineOption );
    static QRegularExpression	txtbrrex( "\\*\\*(.+)\\*\\*", QRegularExpression::MultilineOption );
    static QRegularExpression	txtitrex( "\\*(.+)\\*", QRegularExpression::MultilineOption );
    static QRegularExpression	txtstrex( "~~(.+)~~", QRegularExpression::MultilineOption );
    static QRegularExpression	linkrex( "\\[(.+?)\\]\\((.+?)\\)", QRegularExpression::MultilineOption );
    static QRegularExpression	imgrex( "!\\[(.+?)\\]\\((.+?)\\)", QRegularExpression::MultilineOption );
    static QRegularExpression	nlrex( "\n{2,}", QRegularExpression::MultilineOption );
    static QRegularExpression	hrrex( "^(\\*{3,}|_{3,}|-{3,})$", QRegularExpression::MultilineOption );
    static QRegularExpression	quotrex( "^> ?(.*)$", QRegularExpression::MultilineOption );
    static QRegularExpression	cblockrex( "```([A-Za-z]*)\\s*([^`]+)`{3,}", QRegularExpression::MultilineOption );
    static QRegularExpression	multitabrex( "(<ul>|<ol>)<li>(\t| {4})(.*?)</li>(</ul>|</ol>)", QRegularExpression::MultilineOption );
    static QRegularExpression	inlinecblock( "`(.+?)`", QRegularExpression::MultilineOption );

    /* 替换<>两个符号 */
    htmlText.replace( "<", "&lt;" );
    static QRegularExpression gtrex( "(?<!^)>", QRegularExpression::MultilineOption );
    htmlText.replace( gtrex, "&gt;" );

    /* 将标题转换为HTML标签 */
    htmlText.replace( h1rex, "<h1>\\1</h1>" );
    htmlText.replace( h2rex, "<h2>\\1</h2>" );
    htmlText.replace( h3rex, "<h3>\\1</h3>" );
    htmlText.replace( h4rex, "<h4>\\1</h4>" );
    htmlText.replace( h5rex, "<h5>\\1</h5>" );
    htmlText.replace( h6rex, "<h6>\\1</h6>" );

    /* 将无序列表项转换为HTML标签 */
    htmlText.replace( ulrex, "<ul><li>\\1\\4</li></ul>" );

    /* 将有序列表项转换为HTML标签 */
    htmlText.replace( olrex, "<ol><li>\\1\\4</li></ol>" );

    /* 转换多级列表 */
    for ( int i = 0; i < 5; i++ )
    {
        htmlText.replace( multitabrex, "\\1<li>\\1<li>\\3</li>\\4</li>\\4" );
    }

    /* 将粗体文本转换为HTML标签 */
    htmlText.replace( txtbrrex, "<strong>\\1</strong>" );

    /* 将斜体文本转换为HTML标签 */
    htmlText.replace( txtitrex, "<em>\\1</em>" );

    /* 将删除线文本转换为HTML标签 */
    htmlText.replace( txtstrex, "<s>\\1</s>" );

    /* 将图片转换为HTML标签 */
    htmlText.replace( imgrex, "<img src=\"\\2\" alt=\"\\1\">" );

    /* 将链接转换为HTML标签 */
    htmlText.replace( linkrex, "<a href=\"\\2\">\\1</a>" );

    /* 将分割线转换为HTML标签 */
    htmlText.replace( hrrex, "<hr>" );

    /* 将引用转换为HTML标签 */
    htmlText.replace( quotrex, "<blockquote>\\1</blockquote>" );
    /* 移除多余的引用标签 */
    htmlText.replace( "</blockquote>\n<blockquote>", "<br>" );
    /* 为每个引用添加样式 */
    htmlText.replace( "<blockquote>", "<div class='quote-div' style='background-color: lightgray; border-radius: 10px; padding: 5px;'><blockquote>" );
    htmlText.replace( "</blockquote>", "</blockquote></div>" );

    /* 将代码块转换为HTML标签 */
    QRegularExpressionMatchIterator it = cblockrex.globalMatch( htmlText );
    while ( it.hasNext() )
    {
        QRegularExpressionMatch match	= it.next();
        QString			content = match.captured( 2 ).trimmed();
        content.replace( "\n", "<br>" );
        htmlText.replace( match.capturedStart( 2 ), match.capturedLength( 2 ), content );
    }
    htmlText.replace( cblockrex, "<div class=’code-container‘ style='background-color: #f2f2f2; padding: 10px;'><pre><code class='\\1' style='white-space: pre-wrap;'>\\2</code></pre></div>" );

    /* 将换行符转换为HTML标签 */
    htmlText.replace( nlrex, "<br>" );
    htmlText.replace( "\n", " " );

    /* 移除标题前后多余的换行 */
    htmlText.replace( rmexbrrex, "\\1\\2" );

    /* 移除多余的列表标签 */
    htmlText.replace( "</ul> <ul>", "" );
    htmlText.replace( "</ol> <ol>", "" );

    /* 将内联代码块转换为HTML标签 */
    htmlText.replace( inlinecblock, "<code style='color: blue;'>\\1</code>" );

    qDebug() << htmlText;
    return(htmlText);
}

