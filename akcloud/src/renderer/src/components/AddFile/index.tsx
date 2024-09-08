import { Button, Menu, MenuItem } from '@mui/material'
import Add from '@mui/icons-material/Add'
import { useState } from 'react'
const AddFile = () => {
  const [anchorEl, setAnchorEl] = useState<null | HTMLElement>(null)
  const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
    setAnchorEl(event.currentTarget)
  }
  const handleClose = () => {
    setAnchorEl(null)
  }

  return (
    <>
      <Button
        variant="contained"
        startIcon={<Add />}
        size="small"
        sx={{
          padding: '0px 5px !important',
          height: '40px !important'
        }}
        onClick={handleClick}
      >
        添加文件
      </Button>
      <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={handleClose}>
        <MenuItem onClick={handleClose}>上传文件</MenuItem>
        <MenuItem onClick={handleClose}>上传文件夹</MenuItem>
      </Menu>
    </>
  )
}
export default AddFile
