let &path.="src/include, /usr/include/AL,"
set makeprg=make\ ../build\ all
nnoremap <F4> :make!<cr>
nnoremap <F5> :!.../build/Prog<cr>
