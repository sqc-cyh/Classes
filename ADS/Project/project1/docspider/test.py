import requests

from bs4 import BeautifulSoup

html = '''
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN"
 "http://www.w3.org/TR/REC-html40/loose.dtd">
 <html>
 <head>
 <title>The Complete Works of William Shakespeare
 </title>
 <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
 <LINK rel="stylesheet" type="text/css" media="screen"
       href="/shake.css">
 </HEAD>
 <body bgcolor="#ffffff" text="#000000">

<table width="100%" bgcolor="#CCF6F6">
<tr><td class="play" align="center">The Complete Works of William Shakespeare
</table>

<blockquote>
<img src="shake.gif" vspace=20 hspace=10 height=282 width=222 align=left>

<blockquote>Welcome to the Web's first edition of the Complete Works
of William Shakespeare.  This site has offered Shakespeare's plays and
poetry to the Internet community since 1993.
</blockquote>

<P>For other Shakespeare resources, visit the <A
href="http://shakespeare.palomar.edu/">Mr. William Shakespeare
and the Internet</A> Web site.

<P>The original electronic source for this server was the Complete
Moby(tm) Shakespeare. The HTML versions of the plays provided here are placed in the public domain.

<P><b>5/10/23:</b> Current copy taken from <a href="https://github.com/TheMITTech/shakespeare/">https://github.com/TheMITTech/shakespeare/</a> and hosted by MIT IS&amp;T in a static location.
<P><A href="news.html">Older news items</A>

</blockquote>

<br clear=all>

<p>
<table BORDER CELLPADDING=5 align=center>
<tr ALIGN=CENTER>
<td>
<h2>
Comedy</h2>
</td>

<td>
<h2>
History</h2>
</td>

<td>
<h2>
Tragedy</h2>
</td>

<td>
<h2>
Poetry</h2>
</td>
</tr>

<tr>
<td VALIGN=BASELINE NOWRAP>
<a href="allswell/index.html">
All's Well That Ends Well</a>

<br><a href="asyoulikeit/index.html">
As You Like It</a>

<br><a href="comedy_errors/index.html">
The Comedy of Errors</a>

<br><a href="cymbeline/index.html">Cymbeline</a>

<br><a href="lll/index.html">Love's Labours Lost</a>

<br><a href="measure/index.html">Measure for Measure</a>

<br><a href="merry_wives/index.html">The
Merry Wives of Windsor</a>

<br><a href="merchant/index.html">The Merchant of Venice</a>

<br><a href="midsummer/index.html">A Midsummer Night's Dream</a>

<br><a href="much_ado/index.html">Much Ado About Nothing</a>

<br><a href="pericles/index.html">Pericles, Prince of Tyre</a>

<br><a href="taming_shrew/index.html">Taming of the Shrew</a>

<br><a href="tempest/index.html">The Tempest</a>

<br><a href="troilus_cressida/index.html">Troilus and Cressida</a>

<br><a href="twelfth_night/index.html">Twelfth Night</a>

<br><a href="two_gentlemen/index.html">Two Gentlemen of Verona</a>

<br><a href="winters_tale/index.html">Winter's Tale</a>
</td>

<td VALIGN=BASELINE>
<a href="1henryiv/index.html">Henry IV, part 1</a>

<br><a href="2henryiv/index.html">Henry IV, part 2</a>

<br><a href="henryv/index.html">Henry V</a>

<br><a href="1henryvi/index.html">Henry VI, part 1</a>

<br><a href="2henryvi/index.html">Henry VI, part 2</a>

<br><a href="3henryvi/index.html">Henry VI, part 3</a>

<br><a href="henryviii/index.html">Henry VIII</a>

<br><a href="john/index.html">King John</a>

<br><a href="richardii/index.html">Richard II</a>

<br><a href="richardiii/index.html">Richard III</a></td>

<td VALIGN=BASELINE>
<a href="cleopatra/index.html">Antony and Cleopatra</a>

<br><a href="coriolanus/index.html">Coriolanus</a>

<br><a href="hamlet/index.html">Hamlet</a>

<br><a href="julius_caesar/index.html">Julius Caesar</a>

<br><a href="lear/index.html">King Lear</a>

<br><a href="macbeth/index.html">Macbeth</a>

<br><a href="othello/index.html">Othello</a>

<br><a href="romeo_juliet/index.html">Romeo and Juliet</a>

<br><a href="timon/index.html">Timon of Athens</a>

<br><a href="titus/index.html">Titus Andronicus</a></td>

<td VALIGN=BASELINE>
<em><a href="Poetry/sonnets.html">
The Sonnets</a></em>

<br><em><a href="Poetry/LoversComplaint.html">
A Lover's Complaint</a></em>

<br><em><a href="Poetry/RapeOfLucrece.html">
The Rape of Lucrece</a></em>

<br><em><a href="Poetry/VenusAndAdonis.html">
Venus and Adonis</a></em>

<br><em><a href="Poetry/elegy.html">
Funeral Elegy by W.S.</a></em></td>
</tr>
</table></center>

<hr>

<table width="100%" bgcolor="#CCF6F6">
<tr><td align="center" class="nav">Created by
    <A href="http://www.python.org/~jeremy/">Jeremy Hylton</A>,
    &lt;jeremy@alum.mit.edu&gt;
<tr><td align="center" class="nav">Operated by
    <A href="http://tech.mit.edu/">The Tech</A>, MIT's
    oldest and largest newspaper
<tr><td align="center" class="nav">
    <A href="https://accessibility.mit.edu/">MIT Accessibility</A>
</table>

<!-- Google Analytics code -->
<script src="http://www.google-analytics.com/ga.js"></script>
<script type="text/javascript">
try{
var pageTracker = _gat._getTracker("UA-7382327-2");
pageTracker._trackPageview();
} catch(err) {}
</script>

</body>
</html>
'''
soup = BeautifulSoup(html, "html.parser")

print(soup.a['href'])
