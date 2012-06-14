CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS = services \
        confirm \
        contact_handler \
        src

TRANSLATIONS = meta/callrecorder_en.ts \
    meta/callrecorder_ru.ts \
    meta/callrecorder_tr.ts \
    meta/callrecorder_vi.ts \
    meta/callrecorder_pl.ts \
    meta/callrecorder_da.ts
