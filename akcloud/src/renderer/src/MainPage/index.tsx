import { Box } from '@mui/material'
import Drawer from '../components/Drawer'
import Files from './Files'
import Timer from './Timer'
import Cloud from './Cloud'
import { useState } from 'react'

const components = [<Files />, <Timer />, <Cloud />]

const MainPage = () => {
  const [selectedIndex, setSelectedIndex] = useState(0)
  return (
    <Box sx={{ backgroundColor: '#04002c', height: '100vh', overflow: 'hidden' }}>
      <Box
        display="flex"
        flexGrow={1}
        height={35}
        bgcolor={'#3b4867'}
        sx={{ '-webkit-app-region': 'drag' }}
      ></Box>
      <Box display={'flex'}>
        <Drawer selectedIndex={selectedIndex} setSelectedIndex={setSelectedIndex} />
        {components[selectedIndex]}
      </Box>
    </Box>
  )
}

export default MainPage
