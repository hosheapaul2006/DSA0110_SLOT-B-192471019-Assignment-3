from pathlib import Path
import html
import re
import textwrap
ROOT=Path(__file__).resolve().parents[1]
REPORT=ROOT/'Report'
REPORT.mkdir(exist_ok=True)
URL='https://github.com/hosheapaul2006/DSA0110_SLOT-B-192471019-Assignment-3'

def render_pdf(markdown):
    from reportlab.lib import colors
    from reportlab.lib.enums import TA_CENTER
    from reportlab.lib.pagesizes import A4,landscape
    from reportlab.lib.styles import getSampleStyleSheet,ParagraphStyle
    from reportlab.platypus import (BaseDocTemplate,PageTemplate,Frame,Paragraph,Spacer,PageBreak,
                                   NextPageTemplate,Preformatted,Table,TableStyle,Flowable)
    from reportlab.platypus.tableofcontents import TableOfContents
    from reportlab.lib.utils import ImageReader
    from PIL import Image
    portrait=A4; wide=landscape(A4); margin=42
    styles=getSampleStyleSheet()
    styles.add(ParagraphStyle(name='Body',fontName='Helvetica',fontSize=9.1,leading=13,spaceAfter=6))
    styles.add(ParagraphStyle(name='Small',parent=styles['Body'],fontSize=7.6,leading=10))
    styles.add(ParagraphStyle(name='CodeBlock',fontName='Courier',fontSize=7.8,leading=10.1,spaceAfter=9,textColor=colors.black,
                             backColor=colors.HexColor('#f4f5f6'),borderPadding=6))
    styles['Heading1'].fontSize=17;styles['Heading1'].leading=22;styles['Heading1'].spaceAfter=11
    styles['Heading2'].fontSize=12.5;styles['Heading2'].leading=17
    styles['Heading3'].fontSize=10.5;styles['Heading3'].leading=14
    styles.add(ParagraphStyle(name='CoverTitle',fontName='Helvetica-Bold',fontSize=24,leading=31,alignment=TA_CENTER,spaceAfter=24))
    styles.add(ParagraphStyle(name='CoverLine',fontName='Helvetica',fontSize=12,leading=19,alignment=TA_CENTER,spaceAfter=10))

    class ReportDoc(BaseDocTemplate):
        def afterFlowable(self,flowable):
            if isinstance(flowable,Paragraph) and flowable.style.name=='Heading1' and flowable.getPlainText()!='Contents':
                self.notify('TOCEntry',(0,flowable.getPlainText(),self.page))
    def footer(canvas,doc):
        pageWidth,pageHeight=doc.pagesize
        canvas.setFont('Helvetica',7)
        canvas.setFillColor(colors.HexColor('#4b5563'))
        canvas.drawString(margin,24,'DSA0110 Assignment 3 | D Hoshea Paul | 192471019')
        canvas.drawRightString(pageWidth-margin,24,str(doc.page))
    def portrait_begin(canvas,doc): doc.pagesize=portrait;canvas.setPageSize(portrait)
    def landscape_begin(canvas,doc): doc.pagesize=wide;canvas.setPageSize(wide)
    doc=ReportDoc(str(REPORT/'Assignment3_Report.pdf'),pagesize=portrait,
                  title='DSA0110 - Assignment 3 (CO3)',author='D Hoshea Paul',
                  leftMargin=margin,rightMargin=margin,topMargin=margin,bottomMargin=40)
    def portrait_page(canvas,doc): portrait_begin(canvas,doc);footer(canvas,doc)
    def landscape_page(canvas,doc): landscape_begin(canvas,doc);footer(canvas,doc)
    doc.addPageTemplates([
        PageTemplate('portrait',[Frame(margin,40,portrait[0]-2*margin,portrait[1]-margin-40,id='portrait-frame')],onPage=portrait_page,pagesize=portrait),
        PageTemplate('landscape',[Frame(margin,40,wide[0]-2*margin,wide[1]-margin-40,id='landscape-frame')],onPage=landscape_page,pagesize=wide)])

    def safe(text):
        for old,new in [('–','-'),('—','-'),('’',"'"),('‘',"'"),('“','"'),('”','"'),('→','->'),('₹','Rs '),('\u00a0',' ')]: text=text.replace(old,new)
        return text
    def inline(text):
        text=html.escape(safe(text))
        text=re.sub(r'`([^`]+)`',r'<font name="Courier">\1</font>',text)
        text=re.sub(r'\*\*([^*]+)\*\*',r'<b>\1</b>',text)
        text=re.sub(r'\[([^\]]+)\]\(([^)]+)\)',lambda m: m[1]+' ('+m[2]+')',text)
        return text
    story=[]
    def pb():
        if story and not isinstance(story[-1],PageBreak):story.append(PageBreak())
    def switchpage(template):
        if len(story)>=2 and isinstance(story[-1],PageBreak) and isinstance(story[-2],NextPageTemplate):
            story[-2]=NextPageTemplate(template)
        else:
            story.append(NextPageTemplate(template));pb()
    def para(text,style='Body'): story.append(Paragraph(inline(text),styles[style]))
    class CapturePanel(Flowable):
        def __init__(self,path,width,height):
            Flowable.__init__(self);self.path=str(path);self.width=width;self.height=height
            self.iw,self.ih=Image.open(path).size
        def draw(self):
            scale=self.width/self.iw
            self.canv.saveState()
            clip=self.canv.beginPath();clip.rect(0,0,self.width,self.height)
            self.canv.clipPath(clip,stroke=0)
            self.canv.drawImage(ImageReader(self.path),0,self.height-self.ih*scale,width=self.width,height=self.ih*scale)
            self.canv.restoreState()
    story+=[Spacer(1,105),Paragraph('DSA0110 - Assignment 3 (CO3)',styles['CoverTitle'])]
    for line in ['Student: D Hoshea Paul','Register Number: 192471019','Slot: B','C++17 | Constructors, Destructors and Operator Overloading']:
        story.append(Paragraph(line,styles['CoverLine']))
    story+=[Spacer(1,30),Paragraph('GitHub Repository',styles['CoverLine']),Paragraph(URL,styles['Small']),
            Spacer(1,24),Paragraph('Submission date: not specified in the assignment PDF',styles['CoverLine'])]
    pb();para('Contents','Heading1')
    toc=TableOfContents();toc.levelStyles=[ParagraphStyle(name='TOC0',fontName='Helvetica-Bold',fontSize=10,leading=16,spaceBefore=8),
                                         ParagraphStyle(name='TOC1',fontName='Helvetica',fontSize=8,leading=11,leftIndent=12)]
    story.append(toc);pb()
    # Cover information is already laid out above; retain all remaining Markdown content.
    body=markdown[markdown.index('## Scope, environment and evidence'):]
    lines=body.splitlines();i=0
    while i<len(lines):
        line=lines[i].strip()
        if not line:i+=1;continue
        if line.startswith('```'):
            block=[];i+=1
            while i<len(lines) and not lines[i].strip().startswith('```'):
                raw=safe(lines[i].expandtabs(4))
                block.extend(textwrap.wrap(raw,width=100,replace_whitespace=False,drop_whitespace=False,break_long_words=True,break_on_hyphens=False) or [''])
                i+=1
            story.append(Preformatted('\n'.join(block),styles['CodeBlock']));i+=1;continue
        image=re.match(r'!\[([^\]]*)\]\(([^)]+)\)',line)
        if image:
            switchpage('landscape')
            para(image[1],'Heading2')
            para('Genuine Programiz capture. Upper panel shown where the full capture is taller; the unchanged original file is included in the repository.','Small')
            width=wide[0]-2*margin-12;height=wide[1]-margin-40-75
            story.append(CapturePanel((REPORT/image[2]).resolve(),width,height))
            story.append(NextPageTemplate('portrait'));pb();i+=1;continue
        heading=re.match(r'^(#{1,6})\s+(.+)',line)
        if heading:
            level=len(heading[1]);text=heading[2]
            if level==1:pb()
            para(text,'Heading'+str(min(level,3)));i+=1;continue
        if line.startswith('|'):
            rows=[]
            while i<len(lines) and lines[i].strip().startswith('|'):
                cells=[c.strip() for c in lines[i].strip().strip('|').split('|')]
                if not all(re.fullmatch(r'[:\- ]+',c or '-') for c in cells): rows.append(cells)
                i+=1
            n=max(len(r) for r in rows)
            for row in rows: row.extend(['']*(n-len(row)))
            width=portrait[0]-2*margin-12
            if n==5: widths=[width*x for x in [.07,.28,.25,.31,.09]]
            elif n==7: widths=[width*x for x in [.06,.075,.235,.235,.13,.13,.135]]
            elif n==4 and rows[0][0]=='ID': widths=[width*x for x in [.07,.40,.10,.43]]
            elif n==4: widths=[width*x for x in [.18,.27,.28,.27]]
            else:widths=[width/n]*n
            # Ensure custom fractions sum to the available width.
            factor=width/sum(widths);widths=[w*factor for w in widths]
            table=Table([[Paragraph(inline(c),styles['Small']) for c in row] for row in rows],colWidths=widths,repeatRows=1,hAlign='LEFT')
            table.setStyle(TableStyle([('VALIGN',(0,0),(-1,-1),'TOP'),('BACKGROUND',(0,0),(-1,0),colors.HexColor('#e8edf2')),
                ('GRID',(0,0),(-1,-1),0.3,colors.HexColor('#c6ccd2')),('LEFTPADDING',(0,0),(-1,-1),4),
                ('RIGHTPADDING',(0,0),(-1,-1),4),('TOPPADDING',(0,0),(-1,-1),4),('BOTTOMPADDING',(0,0),(-1,-1),4)]))
            story+=[table,Spacer(1,9)];continue
        paragraph=[line];i+=1
        while i<len(lines) and lines[i].strip() and not lines[i].lstrip().startswith(('#','```','|','![')):
            paragraph.append(lines[i].strip());i+=1
        para(' '.join(paragraph))
    doc.multiBuild(story)

