xmlport 50001 BufferImportTxt
{
    Direction = Import;
    Format = VariableText;
    TextEncoding = UTF8;
    FieldSeparator = '\n';
    UseRequestPage = false;
    Caption = 'BufferImportTxt';
    schema
    {
        textelement(RootNodeName)
        {
            tableelement(ImportBuffer; ImportBuffer)
            {
                fieldelement(LineContent; ImportBuffer."Line Content")
                {
                }
            }
        }
    }
}
