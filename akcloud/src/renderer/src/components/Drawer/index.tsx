import { Box } from '@mui/material'
import { useState } from 'react'
import DrawerItem from './DrawerItem'

const Drawer = () => {
  const [selectedIndex, setSelectedIndex] = useState(0)
  return (
    <>
      <Box sx={{ width: 200, height: '100vh', flexShrink: 0, backgroundColor: '#1f294a' }}>
        <DrawerItem index={0} name={'文件'} onClick={() => setSelectedIndex(0)} />
        <DrawerItem index={1} name={'定时备份'} onClick={() => setSelectedIndex(1)} />
        <DrawerItem index={2} name={'上传网络'} onClick={() => setSelectedIndex(2)} />
      </Box>
    </>
  )
}

export default Drawer
