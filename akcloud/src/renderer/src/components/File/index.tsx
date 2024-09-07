import { Box, Card, CardHeader, Menu, MenuItem, Typography } from '@mui/material'
import { useRef, useState } from 'react'
import MoreHoriz from '@mui/icons-material/MoreHoriz'

import floderImg from '../assets/floder.png'
interface FileProps {
  name: string
  type: string
  size: number
  date: string
  icon: string
}

const File = () => {
  const [anchorEl, setAnchorEl] = useState<null | HTMLElement>(null)
  const handleClick = (event: React.MouseEvent<HTMLDivElement>) => {
    setAnchorEl(event.currentTarget)
  }
  const handleClose = () => {
    setAnchorEl(null)
  }

  return (
    <Box
      sx={{
        px: 2,
        pt: 2,
        pb: 1,
        borderRadius: 4,
        transition: '0.2s',
        '&:hover': { backgroundColor: '#1f294a' },
        '&:hover>:first-child': { visibility: 'visible' },
        cursor: 'pointer',
        position: 'relative'
      }}
    >
      <Box
        sx={{
          width: '30px',
          height: '30px',
          borderRadius: '50%',
          backgroundColor: 'black',
          display: 'flex',
          visibility: 'hidden',
          justifyContent: 'center',
          alignItems: 'center',
          position: 'absolute',
          right: 0,
          mr: 1,
          mt: -1
        }}
        onClick={handleClick}
      >
        <MoreHoriz />
      </Box>
      <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={handleClose}>
        <MenuItem onClick={handleClose}>数据备份</MenuItem>
        <MenuItem onClick={handleClose}>数据还原</MenuItem>
      </Menu>
      <Box sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
        <img src={floderImg} alt="floder" width="100px" />
        <Typography sx={{ fontSize: 14, mb: -1 }}>dir</Typography>
        <Typography sx={{ fontSize: 12, color: 'grey' }}>2024/9/7</Typography>
      </Box>
    </Box>
  )
}
export default File
