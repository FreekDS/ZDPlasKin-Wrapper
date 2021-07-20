!Test

subroutine add(a, b, result)
    integer, intent(in) :: a
    integer, intent(out) :: b
    integer, intent(out) :: result
    result = a + b
end subroutine add

integer function add_old(a, b)
    integer :: a
    integer :: b
    add = a + b
end function add_old
