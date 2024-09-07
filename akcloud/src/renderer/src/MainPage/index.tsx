import { Box } from '@mui/material'
import Drawer from '../components/Drawer'
import Files from './Files'
const MainPage = () => {
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
        <Drawer />
        <Files />
      </Box>
    </Box>
  )
}

export default MainPage
